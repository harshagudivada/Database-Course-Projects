#include<iostream>
#include<cstdlib>
#include<sstream>

#include"minheap.h"
using namespace std;

class Sort : public RIterator {                             //Sort object is like a iterator over a sorted relation 
    public:
	    Sort(const char* dbName,const char* relName,const char* attr)
		:dbName(dbName),type(-1),attrIndex(-1),error(0),schema(NULL),
		finalRun(""),brPtr(NULL),mheap(NUM_BUFFS-5,dbName,type,attrIndex),relName(relName){
		
		    biPtr = new BlockIterator(dbName,relName);
		    set(dbName,relName,attr);
		    if(type == -1 || attrIndex == -1){
			    error = 1;		//error flag is set
		    }
		    if(error == 1)
			    delete mheap.getRootPtr();
	    }

        virtual void open();                                 //sorts the input relation and creates a final sorted relation
        virtual Record next();                               //returns records from sorted relation one by one
        virtual void close();                                //drops the final sorted relation,after Sort object is closed,it can't be accesed
	    void quickSort(char*,int,int);                       //Randomly quicksorts each block internally   
	    void set(const char*,const char*,const char*);       //sets the type and index of sorting attribute and schema from input relation   
	    void swapSlots(int , int ,char* );                   //swap slots within a bufferblock slotted pages during quickSort   

	private:
	    int type;                                            //type is type of attribite on which relation is sorted
	    bool error;		                                     //error flag , if error = 1, sort is stopped
	    MinHeap mheap;                                       //heap is used in merging the sorted blocks
	    Schema* schema;                                      
	    int attrIndex;
	    string finalRun;                                     //finalRun is the name of finalRun relation
	    const char* dbName;
	    const char* relName;
	    BlockIterator* biPtr;                                //used to iterate the input relation blockwise
	    BaseRelIterator* brPtr;                              //used to iterate through temporary and final runs
};
