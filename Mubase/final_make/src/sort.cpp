#include<iostream>
#include"sort.h"
void Sort::open()
{
    char* endTest = NULL;		
	int oldBlockId = -1;
	int run = 1;
	int slotNum = 0;
	char* recBytes = NULL;
	Record rec;
	HeapElem* hePtr = NULL;
	string temp = "";
	stringstream ss;
	RecIterator** recIteratorPPtr = NULL;                               //array of iterator pointers to record iterators
	char* blockPtr[NUM_BUFFS-4] = {0};	                                //array of pointers to blocks
	Relation* relPtr = new Relation(dbName,"r1",true,schema); 
	int outBufferBlock = StorageManager::getFirstBlockId(dbName,"r1");
	char* outBlockBuff = BufferManager::getInstance(dbName)->
                                        pinPage(outBufferBlock);
	SlottedPage* sp = new SlottedPage(outBlockBuff);
	SlottedPage* sPtr = NULL;
	sp->initBlock();
	blockPtr[0] = biPtr->open();	
	int count = 0;
	int frameId = -1;
	char* buffer = NULL;
		
	while( (blockPtr[count])[0] != '\0' ){	                            //loop till end of input relation blockwise
		while(count<(NUM_BUFFS-6)  && (blockPtr[count])[0] != '\0'){
			count++;
			blockPtr[count] = biPtr->next();						
		}		
		count = 0;
		
		while(blockPtr[count] != NULL && (blockPtr[count])[0] != '\0'){	 //quicksort all the blocks of buffer in relation   
			sPtr = new SlottedPage(blockPtr[count]);						
			quickSort( blockPtr[count],0,(sPtr->totalSlots()-1) );		
			delete sPtr;
			count++;			
		}
		
		recIteratorPPtr = new RecIterator*[NUM_BUFFS]();                //create an array of pointers to record iterators
		
		for(int i =0;i < count;i++){                                    //fill the  heap with first records of each sorted block
		    ss.clear();
		    temp.clear();				
			buffer = BufferManager::getInstance(dbName)->getBuffer();
			frameId = ( blockPtr[i] - buffer)/BLOCK_SIZE;					
			recIteratorPPtr[frameId] = new RecIterator(dbName,relName,blockPtr[i]);			
			recIteratorPPtr[frameId]->open();						
			rec = recIteratorPPtr[frameId]->next();
									
			switch(type){
				case 1:
				    ss << (rec.getInt(attrIndex));
				    ss >> temp;
				    break;
				    
				case 2:
				    ss << (rec.getFloat(attrIndex));
				    ss >> temp;
				    break;
				    
				case 3:
				    temp = (rec.getString(attrIndex));
				    break;
				    
				 default:
				    cout<<"Invalid type for comparison\n";return ;    
		    }
		    
			slotNum = ( ( recIteratorPPtr[frameId]->getPresentSlot() ) - 1 );				
			hePtr = new HeapElem(blockPtr[i],slotNum,temp,type);			
			mheap.insert(hePtr);
			delete hePtr;
		}
		
		while (1){			                                                    //extract min from heap and insert next record from correspondingblock
			recBytes = mheap.stepRec(recIteratorPPtr);                          //store the extracted records into a run
			if(recBytes == NULL){			       
				BufferManager::getInstance(dbName)->unpinPage(outBufferBlock, true);
				BufferManager::getInstance(dbName)->forcePage( BufferManager::getInstance( dbName )     
				->getBlockInFrame(outBufferBlock) );                                         
				delete sp;
				break;
			}			
			
			if(sp->storeRecord(recBytes) == -1){			    				//if output buffer block is full,
				oldBlockId = outBufferBlock;        		                    //write to disk, and pin another block   
        		BufferManager::getInstance(dbName)->unpinPage(oldBlockId, true);
				BufferManager::getInstance(dbName)->forcePage( BufferManager::getInstance( dbName )
				->getBlockInFrame(oldBlockId) );
				outBufferBlock = StorageManager::allocateBlock(dbName);				
				sp->setNextBlockId(outBufferBlock);				
				outBlockBuff = BufferManager::getInstance( dbName )->pinPage( outBufferBlock );
				delete sp;
		        sp = new SlottedPage(outBlockBuff);
				sp->initBlock();
		        sp->storeRecord(recBytes);                       
			}
        }
        
		for(int j=0;j<count;j++){		                                                
			delete recIteratorPPtr[j];		
		}
				
		if( (blockPtr[count]) != NULL && blockPtr[count][0] =='\0'){            //check for completed relations
			break;}
			
        endTest = biPtr->next();
        
        if( endTest != NULL && endTest[0] == '\0' ){
            break;
        }
                			
		run++;		
		temp.clear();
		ss.clear();
		ss << run;
		ss >> temp;
		temp = "r"+temp;
		delete relPtr;		
		relPtr = new Relation(dbName,&temp[0],true,schema);		                //create new runs
		outBufferBlock = StorageManager::getFirstBlockId(dbName,&temp[0]);
		outBlockBuff = BufferManager::getInstance(dbName)->
                                        pinPage(outBufferBlock);                         
		sp = new SlottedPage(outBlockBuff);
		sp->initBlock();
		blockPtr = {0};  
		blockPtr[0] = endTest;
		count = 0;		      
	}
	biPtr->close();
	//----------------------------------------- first level runs created-------------------------------------------------------------------//
	    
	//----------------------------------------- Merge phase started-------------------------------------------------------------------//    
	
	count  = 0;
	int mergeCount = 0;
	int remRun = run;
	int mergesize;
	BaseRelIterator* ptr = NULL;
	BaseRelIterator** bIteratorPPtr = new BaseRelIterator*[NUM_BUFFS]();  
		
	while(remRun > 1){                                  //merge till we get a single run
		mergeCount++;
		mergesize = min(remRun,NUM_BUFFS-5);
		temp.clear();
		ss.clear();
		run++;
		ss << run;
		ss >> temp;
		temp = "r"+temp;
		delete relPtr;
		relPtr = new Relation(dbName,&temp[0],true,schema);                 //create next level of runs
		outBufferBlock = StorageManager::getFirstBlockId(dbName,&temp[0]);
	    outBlockBuff = BufferManager::getInstance(dbName)->
                                        pinPage(outBufferBlock);
	    sp = new SlottedPage(outBlockBuff);
    	sp->initBlock();
			
		for(int i =0;i < mergesize ;i++){                                   //fill the heap with records from each run
			temp.clear();
			ss.clear();
			ss << ( (mergeCount-1)*(NUM_BUFFS-5)+i+1 );
			ss >> temp;
			temp = "r"+temp;			
			ptr = new BaseRelIterator(dbName,&temp[0]);
			ptr->open();
			frameId = (BufferManager::getInstance(dbName))->getBlockInFrame(ptr->getCurBlock());  		
			bIteratorPPtr[frameId] = ptr;
			rec = bIteratorPPtr[frameId]->next();			
			ss.clear();
			temp.clear();
			
			switch(type){
				case 1:
					ss << (rec.getInt(attrIndex));
					ss >> temp;
					break;
				case 2:
					ss << (rec.getFloat(attrIndex));
					ss >> temp;
					break;
				case 3:
					temp = (rec.getString(attrIndex));
					break;
				default:
				    cout<<"Invalid type for comparison\n";return ;	
			}	
					
			hePtr = new HeapElem(ptr->getCurBlockBuff(),1,temp,type);
			mheap.insert(hePtr);
			delete hePtr;
		}	
		while(1){		                                                    //extract min from heap and insert next record from corresponding run
			recBytes = mheap.stepBri(bIteratorPPtr);                        //store the extracted records into a run
			  			
			if(recBytes == NULL){			    
				BufferManager::getInstance(dbName)->unpinPage(outBufferBlock, true);
				BufferManager::getInstance(dbName)->forcePage( BufferManager::getInstance( dbName )
				->getBlockInFrame(outBufferBlock) );
				delete sp;
				break;
			}			
			
			if(sp->storeRecord(recBytes) == -1){			   
				oldBlockId = outBufferBlock;
				outBufferBlock = StorageManager::allocateBlock(dbName);				
				sp->setNextBlockId(outBufferBlock);				
				BufferManager::getInstance(dbName)->unpinPage(oldBlockId, true);				
				BufferManager::getInstance( dbName )->forcePage(BufferManager::getInstance( dbName )
				->getBlockInFrame(oldBlockId) );
				outBlockBuff = BufferManager::getInstance( dbName )->
		                                   pinPage( outBufferBlock);		                                
				delete sp;
			    sp = new SlottedPage(outBlockBuff);
				sp->initBlock();
			    sp->storeRecord(recBytes);                       
			}			
		}
				
		remRun = remRun - (NUM_BUFFS-5) + 1;
			
		for(int i =0;i < mergesize ;i++){                                   //drop the latest merged runs
			temp.clear();
			ss.clear();
			ss << ( (mergeCount-1)*(NUM_BUFFS-5)+i+1 );
			ss >> temp;
			temp = "r"+temp;
			StorageManager::dropObject(dbName,&temp[0]);  
		}		
	}	
	ss.clear();
	ss << run;
	ss >> finalRun;
	finalRun = "r"+finalRun;
	brPtr = new BaseRelIterator(dbName,&finalRun[0]);
	brPtr->open();
}


Record Sort::next(){
	return (brPtr->next());
}

void Sort::close(){		//final run is deleted    
    StorageManager::dropObject(dbName,&finalRun[0]);
}	

//check if the sorting attribute is valid or not and set attrIndex and type of atribute
void Sort::set(const char* dbName,const char* relName,const char* attr){    	
	BaseRelIterator* riPtr = new BaseRelIterator(dbName,"SYSCOLUMN");	
	riPtr->open();	
	Record rec = riPtr->next();				
	char* recBytes = rec.getRecBytes();
	int i;	
	int tempIndex = -1;
	string temp = "";
	Relation rel(dbName, relName);	
	while( recBytes != NULL){ 		                                //validate the relation,sorting attribute,set schema,type and index of sorting 
		i = 0;                                                      //attribute                
		while(recBytes[i] != '|'){			
			temp = temp + recBytes[i];
			i++;
		}
		i++;
		if(temp == relName){						
			temp="";
			while(recBytes[i] != '|'){				
				temp = temp + recBytes[i];
				i++;
			}
			tempIndex = (atoi(temp.c_str())) ;
			i++;
			temp="";
			while(recBytes[i] != '|'){				
				temp = temp + recBytes[i];
				i++;
			}
			if(temp == attr){				
				i++;
				temp = "";
				while(recBytes[i] != '\0'){					
					temp = temp + recBytes[i];
					i++;
				}
				this->type = atoi(temp.c_str());
				this->mheap.setType(this->type);				
				this->schema = rel.getSchema();				
				this->attrIndex = tempIndex;
				this->mheap.setIndex(tempIndex);
				riPtr->close();						
            	delete riPtr;
				return;
			}
		}		
		rec = riPtr->next();
		recBytes = rec.getRecBytes();			
		temp="";
	}
	delete riPtr;
}

void Sort::quickSort(char* bba,int start,int end){	            //Randomised quick sort
	SlottedPage sp(bba);
	string s1;
	string s2;
		
	if(start<end)				
	{		
		srand(time(0));					                        //seed random number generator using system time settings
		int i=( (rand()%(end-start+1)) + start );			    //randomise the pivot
		Sort::swapSlots(i,end,bba);				                //swap the pivot and last element in the sub array
		while(readShort(bba, (3*SIZE_SHORT)+(end-1)*2*SIZE_SHORT) == -1){
			end--;
		}
		int lend=(start-1);		                                //boundaries of smaller array
		Record* recPtr1;
		Record* recPtr2;
		string temp;
		stringstream ss;
		for(int c=start;c<=end;c++)			                      //loop through array
		{
			while(readShort(bba, (3*SIZE_SHORT)+(c)*2*SIZE_SHORT) == -1){
				c++;
			}
			recPtr1 = new Record( (sp.getRecord(c+1)),schema );   // have to use slot num, not slot attrIndex
			recPtr2 = new Record( (sp.getRecord(end+1)),schema );
			ss.clear();
			temp.clear();			
			
			if( !( (*recPtr1).greater(*recPtr2,attrIndex) ) ){
				Sort::swapSlots(lend+1,c,bba);		              //if a element is smaller than pivot,swap it with head of bigger array
				lend++;
			}
		}
		quickSort(bba,start,lend-1);			                   //recursively call RQSORT on smaller and larger sub arrays
		quickSort(bba,lend+1,end);
	}
}

// a and b are slot attrIndex values, not slot numbers
void Sort::swapSlots(int a, int b,char* buffBlockPtr){	
	short temp=-1;
	temp = readShort(buffBlockPtr, (3*SIZE_SHORT)+(a)*2*SIZE_SHORT);
	writeShort(buffBlockPtr, (3*SIZE_SHORT)+(a)*2*SIZE_SHORT , (readShort(buffBlockPtr, (3*SIZE_SHORT)+(b)*2*SIZE_SHORT)));
	writeShort(buffBlockPtr, (3*SIZE_SHORT)+(b)*2*SIZE_SHORT , temp);

	temp = readShort(buffBlockPtr, (3*SIZE_SHORT)+(a)*2*SIZE_SHORT+SIZE_SHORT);
	writeShort(buffBlockPtr, (3*SIZE_SHORT)+(a)*2*SIZE_SHORT+SIZE_SHORT , (readShort(buffBlockPtr, (3*SIZE_SHORT)+(b)*2*SIZE_SHORT+SIZE_SHORT)));
	writeShort(buffBlockPtr, (3*SIZE_SHORT)+(b)*2*SIZE_SHORT+SIZE_SHORT , temp);
}
