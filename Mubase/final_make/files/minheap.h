#ifndef MH_H
#define MH_H
#include<sstream>
#include"rm.h"
#include"sputil.h"

// Block Iterator to read blockwise in to the buffer initially
// to call quicksort on each block
class BlockIterator{
	public:
		// Block Iterator constructor
	    BlockIterator(const char* dbName,const char* relName){
		    this->curBlock  = 0;
		    this->dbName = dbName;
		    this->relName = relName;
		    this->curBlockBuff = NULL;
		    this->sp = NULL;
	    }

		// Open function
	    char* open(){
		    curBlock = StorageManager::getFirstBlockId(dbName, relName);	// Get first blockid of relation
            curBlockBuff = BufferManager::getInstance(dbName)->
                                            pinPage(curBlock);				// Pin the first block of relation to buffer
        	sp = new SlottedPage( curBlockBuff );							// Slotted page object for the pinned block
		    return curBlockBuff;
	    }

		// Next function
	    char* next(){
	        int oldBlock = curBlock;
		    curBlock = sp->getNextBlockId();			// Get next block id
		    if(curBlock == -1){		       
			    //cout<<"End of Relation\n";				// EOF if next block is -1
			    return "\0";
		    }		    
		    curBlockBuff = BufferManager::getInstance(dbName)->
                                            pinPage(curBlock);	// Pin the next block
		    if(curBlockBuff == NULL){				// Unable to pin
				cout<<"an not next block because of failed pinning"<<endl;
				return NULL;
			}
		    delete sp;
		    sp = new SlottedPage(curBlockBuff);					// Slotted page object for the pinned block
		    return curBlockBuff;
	    }
	    
	    void close(){
		    delete sp;			// Delete the dynamically created object
	    }
	    
	private:
	    int curBlock;			// Current block id
	    SlottedPage* sp;		// Pointer to slotted page
	    const char* dbName;		// Pointer to Data Base name
	    const char* relName;	// Pointer to Relation name
	    char* curBlockBuff;		// Pointer to the address of buffer block which is pinned
};

// Record Iterator to read recordwise in a given block
class RecIterator : public RIterator{
	public:
		// Record Iterator constructor
		RecIterator(const char* dbName,const char* relName,char* block)
			:presentSlot(1),blockBuff(block),sp(NULL),tslot(0){
			Relation r( dbName, relName );
			this->schema = r.getSchema();		
		}

		// Default constructor
		RecIterator(){;}

		// Open function
		void open(){
			this->sp = new SlottedPage(this->blockBuff);				// slotted page object for the block
			short n    = readShort(this->blockBuff, (short) 0);			// total no.of records in the block
			short mideofs = readShort(this->blockBuff,(BLOCK_SIZE-4));	// no.of deleted free slots in slot array
			this->tslot = n + mideofs;									// occupied slots + deleted free slots
		}
		
		// Next function
		Record  next(){
			char* recBytes = sp->getRecord(presentSlot);		// Record of the present slot
			if(recBytes == NULL){				
				return Record(NULL,schema);						// If slot number is lessthan 0 or greater than 
			}													// the size of slot array return NULL

			while(recBytes[0] == '\0'){							// If the slot number is of deleted free slot, \0 will be given
				presentSlot++;									// Increment till a record is found
				recBytes = sp->getRecord(presentSlot);
				if(recBytes == NULL){					
					return Record(NULL,schema);					// End of the block
				}
			}
			presentSlot++;			
			return Record(recBytes,schema);
		}

		// Close function
		void close(){
			delete sp;
		}

		// get function for presentslot
		int getPresentSlot(){
			return presentSlot;
		}

		// get function for blockBuff
		char* getBlockBuff(){
			return blockBuff;
		}
	private:
		int tslot;			// Occupied + deleted free slots
		Schema* schema;		// Schema
		int presentSlot;	// Present slot
		SlottedPage* sp;	// Pointer to slotted page
		char* blockBuff;	// Address of block on which iteration has to be called
};

// Heap Element
class HeapElem{	
	public:
		// Constructor
		HeapElem(char* buff,int sn,string atb,int t)
			:buffBlockAdd(buff),slotNo(sn),attrib(atb),type(t){
		}
		
		// Default constructor
		HeapElem(){		
			buffBlockAdd = NULL;
			slotNo = -1;
			attrib = "";
		}
		
		// <= operator to compare two heap elements on attribute
		bool operator<= (const HeapElem& right){
			stringstream ss;
			int tempInt1,tempInt2;float tempFloat1,tempFloat2;
			switch(type){
				case 1:
					ss << attrib;
					ss >> tempInt1;
					ss.clear();
					ss << right.attrib;
					ss >> tempInt2;
					ss.clear();
					return (tempInt1 <= tempInt2);
					break;

				case 2:
					ss << attrib;
					ss >> tempFloat1;
					ss.clear();
					ss << right.attrib;
					ss >> tempFloat2;
					ss.clear();
					return (tempFloat1 <= tempFloat2);
					break;
	
				case 3:
					return attrib <= right.attrib;
				
				default:
				    cout<<"Invalid type for comparison\n";return 0;	
			}			
		}
		
		// > operator to compare two heap elements on attribute
		bool operator> (const HeapElem& right){
			stringstream ss;
			int tempInt1,tempInt2;float tempFloat1,tempFloat2;			
			switch(type){
				case 1:
					ss << attrib;
					ss >> tempInt1;
					ss.clear();
					ss << right.attrib;
					ss >> tempInt2;
					ss.clear();					
					return (tempInt1 > tempInt2);					

				case 2:
					ss << attrib;
					ss >> tempFloat1;
					ss.clear();
					ss << right.attrib;
					ss >> tempFloat2;
					ss.clear();
					return (tempFloat1 > tempFloat2);				
	
				case 3:
					return (attrib > right.attrib);
					
				default:
				    cout<<"Invalid type for comparison\n";return 0;					
			}			
		}

		// Get function for buffBlockAdd
		char* getBuffBlockAdd(){
			return buffBlockAdd;
		}

		// Get function for attrib
		string getAttrib(){
			return attrib;
		}

		// Get function for slotNo
		int getSlotNo(){
			return slotNo;
		}
	private:
	char* buffBlockAdd;	// Address of the block
	int slotNo;			// slot number to the corresponding attrib
	string attrib;		// attribute value
	int type;			// Type(Int, Float, String)
};

// MinHeap
class MinHeap {

	public:
		// Constructor
	    MinHeap(int hsize,const char* dn,int t,int ind)
		    :size(hsize),dbName(dn),type(t),index(ind){		
		    rootPtr = new HeapElem[hsize];
		    last = -1;
	    }

		// Default constructor
	    MinHeap(){;}

		// Function to get a pointer to root
	    HeapElem* getRootPtr(){
		    return rootPtr;
	    }
	    
	    HeapElem extractMin();					// Function Extract minimum
	    void insert(HeapElem* );				// Function Insert
	    void minHeapify(int );					// heapify function
	    int min(int ,int );						// Min function
	    void swap(HeapElem *,HeapElem *);		// Swap two heap elements
	    char* stepRec(RecIterator**);	// function to extract min and insert if needed, when record iterator is used
		char* stepBri(BaseRelIterator**);// function to extract min and insert if needed, when baserel iterator is used
	    void setType(int);						// set function for type
	    void setIndex(int);						// set function for index
	    
	private:
	    HeapElem* rootPtr;	// Pointer to root
	    int last;			// Index of the last occupied place
	    int size;			// Size of the heap
	    const char* dbName;	// Pointer to
	    int type;			// type
	    int index;			// index
};

	
#endif
