
#include "global.h"
#include "bt.h"
#include "iter.h"
#include "rm_constants.h"
#include "sp.h"

#define REL_TYPE 1

using namespace std;

class Relation;

class Schema {
    int numAttrs;
    int attrType[MAX_ATTRS];
    char *attrName[MAX_ATTRS];
    public:
    // Add useful methods here
    void setSchema( char* schema );
    char *getSchema();
    int stringToType( char *type );
    int getAttrType( int index );
    char *getAttrName( int index );
    int getNumAttrs();
	int getAttIndex( char *attriName );
};


class Record {
    
    private:
    char *recBytes;
    Schema *schema;
    //char *hold;//holds the character array for getstring() to decrease memory usage
    
    public: 
    Record(char *recBytes, Schema *schema);
	Record();
    //attrIndex starts from ZERO
    int getInt(int attrIndex);
    char *getString(int attrIndex);
    float getFloat(int attrIndex);
    char *getRecBytes();
    Schema *getSchema() {
        return schema;
    }

	bool equals( Record right, int attrIndex );
    bool less( Record right,  int attrIndex);
    bool greater( Record right,  int attrIndex );
    //...
};


// Iterator for a relation.
class BaseRelIterator : public RIterator {
    // Here go the attributes to maintain iterator state
    const char *dbName;
    const char *relName;
    char *curBlockBuff;
    int curBlock;
    int curSlot;
    SlottedPage *sp;
    Schema *schema;

    public:

    BaseRelIterator(const char *dbName, const char *relName);

    // Open the iterator, initializing its state
    void open() {
        curBlock = StorageManager::getFirstBlockId(dbName, relName);
        curBlockBuff = BufferManager::getInstance(dbName)->
                                        pinPage(curBlock);
    	sp = new SlottedPage( curBlockBuff );
    }

    // Returns the next record, NULL if no more records.
    Record next();

    // Close the iterator, unpin any pinned buffer blocks
    void close();
	int getCurSlot(){
		return curSlot;
	}
	int getCurBlock(){
		return curBlock;
	}
	char* getCurBlockBuff(){
	    return curBlockBuff;
	}
	/*Schema* getSchema(){
		return schema;
	}*/
};

class Predicate {
    public:
        virtual bool evaluate( Record * ) = 0;
        virtual RIterator *evaluate(Relation *rel) = 0;
};

/**
 * implemented in iterator.cpp
 */
class PredIterator : public RIterator {
    private:
        BaseRelIterator *itr;
        const char *dbName;
        const char *relName;
        Predicate *predicate;

    public:
        PredIterator( const char *dbName, const char *relName, Predicate *p ) {
            this->dbName = dbName;
            this->relName = relName;
            this->predicate = p;
        }
        void open();
        Record next();
        void close();
};

/**
 * implemented in iterator.cpp
 */
class BTIterator : public RIterator {
    private:
        BPlusTree *bt;
        int attrIndex;
        Relation* rel;
        char *value;
        bool first;


    public:
        BTIterator(Relation *rel, int attrIndex, char *value);
        void open();
        Record next();
        void close();
};

class Relation {    
    const char *dbName;
    const char *relName;
    bool *hasIndex;
    Schema *schema;
    
    public:
    Relation(const char *dbName, const char *relName, bool create,
             Schema *schema) {	
        this->dbName  = dbName;
        this->relName = relName;

        if(create) {
            assert(schema != NULL);
            StorageManager::createObject(dbName, relName);
           
            int sysObjectId =  StorageManager::getFirstBlockId(dbName, "SYSOBJECT");
            char *sysObjPage = BufferManager::getInstance(dbName)->
                                        pinPage(sysObjectId); 
                                      
            SlottedPage sp(sysObjPage);
            
            char sysObjBytes[OBJNAME_SIZE+2+3];
            int numAttrs = schema->getNumAttrs();
            sprintf( sysObjBytes, "%s|%d|%d", relName, REL_TYPE, numAttrs );
            sp.storeRecord( sysObjBytes );
            BufferManager::getInstance(dbName)->unpinPage(sysObjectId, true);

            int sysColId =  StorageManager::getFirstBlockId(dbName, "SYSCOLUMN");
            
            char *sysColPage = BufferManager::getInstance(dbName)->
                                        pinPage(sysColId);
                                                    
            SlottedPage sp1(sysColPage);
            
            char sysColBytes[numAttrs][2*OBJNAME_SIZE+7];
            for ( int i=0; i<numAttrs; i++ ) {
            	sprintf( sysColBytes[i], "%s|%d|%s|%d", relName, i, schema->getAttrName(i), schema->getAttrType(i) );
            	sp1.storeRecord( sysColBytes[i] );
            }
            BufferManager::getInstance(dbName)->unpinPage(sysColId, true);
            
            int relId =  StorageManager::getFirstBlockId( dbName, relName );
            char *relPage = BufferManager::getInstance( dbName )->
                                        pinPage( relId );
            SlottedPage spr( relPage );
            spr.initBlock();
            BufferManager::getInstance(dbName)->unpinPage(relId, true);
        }
    }
    
    Relation( const char *dbName, const char *relName ){	
    	this->dbName = dbName;
    	this->relName = relName;
    }

    RIterator *openScan() {
            return new BaseRelIterator(dbName, relName);
        }

        // Predicated scan. Should make use of the B+ tree index
        // when an index is available on the required column(s).
        RIterator *openScan(Predicate *p);

        int addRecord(Record rec);

        int deleteRecord(int recId);

        Schema *getSchema();

        //builds index on an attribute
        void buildIndex( int attrIndex );

        bool hasIndexOn(int index) {
            return hasIndex[index];
        }

        const char *getRelName() {
            return relName;
        }

        const char *getDbName() {
            return dbName;
        }
};
