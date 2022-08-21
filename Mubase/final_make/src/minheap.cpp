#include"minheap.h"

// Function StepRec to extract min and insert if present, when record iterator is used
char* MinHeap::stepRec(RecIterator** pptr){				
	if(last < 0){			
		return NULL;		// Heap is empty
	}
	string str = "";
	stringstream ss;		
	HeapElem temp = extractMin();					// Extract min from heap
	char* buffBlockAdd = temp.getBuffBlockAdd();	// Address of the block in buffer corresponding to the heap element
	SlottedPage sp(buffBlockAdd);					// slotted page object for that block
	char* buffer = BufferManager::getInstance(dbName)->getBuffer();	// starting address of the buffer
	int frameId = ( buffBlockAdd - buffer)/BLOCK_SIZE;				// frameid for the block in buffer
	RecIterator* rit = pptr[frameId];		// Pointer to Record iterator for the corresponding buffer block
	Record rec = rit->next();				// Next record
	if(rec.getRecBytes() == NULL){			// Block is completely iterated. End of block. unpin it
		BufferManager::getInstance(dbName)->unpinPage(rit->getBlockBuff(), false);
	}
	else{
		// Switch to get value of the attribute, given index
		switch(type){
			case 1:
				ss << (rec.getInt(index));
				ss >> str;
				break;
			case 2:
				ss << (rec.getFloat(index));
				ss >> str;
				break;
			case 3:
				str = (rec.getString(index));
				break;
		}
		// create heap element to insert
		HeapElem* hePtr = new HeapElem(buffBlockAdd,(rit->getPresentSlot()-1),str,type); 
		insert(hePtr);
	}
	// Return the record corresponding to extracted heapelem
	return ( sp.getRecord(temp.getSlotNo()) );
}

// Function StepBri to extract min and insert if present, when baserel iterator is used
char* MinHeap::stepBri(BaseRelIterator** pptr){
	if(last < 0){
		return NULL;
	}
	string str = "";
	stringstream ss;		
	HeapElem temp = extractMin();				// Extract min from heap
	char* buffBlockAdd = temp.getBuffBlockAdd();// Address of the block in buffer corresponding to the heap element
	SlottedPage sp(buffBlockAdd);				// slotted page object for that block
	char* buffer = BufferManager::getInstance(dbName)->getBuffer();	// starting address of the buffer
	int frameId = ( buffBlockAdd - buffer)/BLOCK_SIZE;				// frameid for the block in buffer
	BaseRelIterator* bite = pptr[frameId];		// Pointer to baserel iterator for the corresponding buffer block
	Record rec = bite->next();					// Next record
	pptr[ ( ( (bite->getCurBlockBuff()) - buffer )/BLOCK_SIZE ) ] = bite;
												// Resetting the index of pointer to baserel iterator
	if(rec.getRecBytes() != NULL){
		// Switch to get value of the attribute, given index
		switch(type){
			case 1:
			    
				ss << (rec.getInt(index));
				ss >> str;
				break;
			case 2:
			    
				ss << (rec.getFloat(index));
				ss >> str;
				break;	
			case 3:
				str = (rec.getString(index));
				break;
		}
		// create heap element to insert
		HeapElem* hePtr = new HeapElem(bite->getCurBlockBuff(),(bite->getCurSlot()-1),str,type);
		insert(hePtr);
		
	}
	// Return the record corresponding to extracted heapelem
	return ( sp.getRecord(temp.getSlotNo()) );
}

// ExtractMin Function
HeapElem MinHeap::extractMin()
{			
	HeapElem temp=rootPtr[0];		//take the first element from the heap
	rootPtr[0]=rootPtr[last];		//take the last element and put it at root of the heap, COPY CONSTRUCTOR MAY BE REQUIRED
	last--;							//heap size is reduced by one		
	MinHeap::minHeapify(0);			//call MinHeapify on root
	return temp;			
}

// Insert function
void MinHeap::insert(HeapElem* e)			
{		
	if(last<size){				//check if heap is alredy full

		rootPtr[last+1]=(*e);		//first,insert at the end of heap
		last++;			
		int temp=last;
		while( rootPtr[temp/2] > rootPtr[temp] ){		//percolate up if parent > child

			swap(&rootPtr[temp/2],&rootPtr[temp]);
			temp=(temp/2);
		}
	}
	else{
		cout<<"Heap is full\n";exit(1);  //change this
	}
}

// Min heapify
void MinHeap::minHeapify(int k)
{
	int v=0;
	if(last <= 0)return;	//if there are zero or only one element in heap, furthur hepaification is not required,return
	if(last == 1)v=1;		//if only two elements,consider the child 
	else{
		if( ((2*k) + 1)<=last){						//check for existence of left child
			if( ((2*k) + 2)<=last){					//check for existence of right child
				v=min( ((2*k) + 1),((2*k) + 2) );	//if both children are present,consider the minium for comparison
			}										//with parent
			else{
				v= ( (2*k) + 1 );	//if only left child exists, consider it for comparison
			}
		}
		else						//if no children,return
		{	
			return;
		}
	}
	if(rootPtr[k] <= rootPtr[v])	
	return;
	else
	{										//if parent > child,swap them
		swap(&rootPtr[k],&rootPtr[v]);
		minHeapify(v);						//call minheapify on present child
	}
}

//swap for HeapElem
void MinHeap::swap(HeapElem *x,HeapElem *y)
{
	HeapElem temp=*x;
	*x=*y;
	*y=temp;
}


// Min of two heapelems given their indices
int MinHeap::min(int x,int y){
	if(rootPtr[x].getAttrib() <= rootPtr[y].getAttrib()) return (x);
	else return (y);
}

// ste function for type
void MinHeap::setType(int t){
	type = t;
}

// set function for index
void MinHeap::setIndex(int i){
	index = i;
}

