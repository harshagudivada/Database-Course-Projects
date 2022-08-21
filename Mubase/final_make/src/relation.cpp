#include "rm.h"

Schema *Relation::getSchema() {
    	
	int sysColId =  StorageManager::getFirstBlockId(dbName, "SYSCOLUMN");
	    	
	char *sysColPage = BufferManager::getInstance(dbName)->
                                        pinPage(sysColId);
                                            	
	SlottedPage sp(sysColPage);
	short int slotNum = 1;
	char attrName[MAX_ATTRS][OBJNAME_SIZE];
	int attrType[MAX_ATTRS];
	int numAttrs = 0;
	while( true ) {
		
		char *recBytes = sp.getRecord( slotNum );
		if ( recBytes == NULL )
			break;
		if ( recBytes[0] == '\0' ) {
			slotNum++;
			continue;
		}
		int i = 0;
		for ( i=0; recBytes[i] != '|'; i++ ) {
			if ( relName[i] != recBytes[i] )
				break;
		}
		if ( recBytes[i] == '|' ) {
			i++;
			//to store int strings (column index and col type)
			char colIndex[10];
			for ( int j=0; recBytes[i] != '|'; j++ ) {
				colIndex[j] = recBytes[i];
				colIndex[j+1] = '\0';
				i++;
			}
			int attrIndex = atoi(colIndex);
			i++;
			for ( int j=0; recBytes[i] != '|'; j++ ) {
				attrName[attrIndex][j] = recBytes[i];
				attrName[attrIndex][j+1] = '\0';
				i++;
			}
			i++;
			for ( int j=0; recBytes[i] != '\0'; j++ ) {
				colIndex[j] = recBytes[i];
				colIndex[j+1] = '\0';
				i++;
			}
			attrType[attrIndex] = atoi(colIndex);
			numAttrs++;
		}
		slotNum++;
	}
	
	char schemaBytes[numAttrs*OBJNAME_SIZE];
	int count = 0;
	for ( int i=0; i<numAttrs; i++ ) {
		for( int j=0; attrName[i][j] != '\0'; j++ ){
			schemaBytes[count] = attrName[i][j];
			count++;
		}
		schemaBytes[count] = ':';
		count++;
		switch( attrType[i] ){
			case INT_TYPE :{
				char *temp = "int";
				for( int j=0; temp[j] != '\0'; j++ ){
					schemaBytes[count] = temp[j];
					count++;
				}
				break;
			}
			case FLOAT_TYPE :{
				char *temp = "float";
				for( int j=0; temp[j] != '\0'; j++ ){
					schemaBytes[count] = temp[j];
					count++;
				}
				break;
			}
			case STRING_TYPE :{
				char *temp = "string";
				for( int j=0; temp[j] != '\0'; j++ ){
					schemaBytes[count] = temp[j];
					count++;
				}
				break;
			}
		}
		schemaBytes[count] = '|';
		count++;
	}
	schemaBytes[count-1] = '\0';
	Schema *schema =  new Schema;
	schema->setSchema(schemaBytes);
	BufferManager::getInstance(dbName)->unpinPage(sysColId, true);
	return schema;
}

int Relation::addRecord( Record rec ) {
    int relId =  StorageManager::getFirstBlockId( dbName, relName );
    char *relPage = BufferManager::getInstance( dbName )->
                    pinPage( relId );
    SlottedPage *sp = new SlottedPage( relPage );
    short slotNum = sp->storeRecord( rec.getRecBytes() );
    short blockId = relId;
    int oldBlockId = blockId;
    int recId;
    while ( slotNum == -1 ) {
        oldBlockId = blockId;
        blockId = sp->getNextBlockId();
        if( blockId == -1 ) {
            blockId = StorageManager::allocateBlock(dbName);
            sp->setNextBlockId(blockId);
            BufferManager::getInstance(dbName)->unpinPage(oldBlockId, true);
            relPage = BufferManager::getInstance( dbName )->
                      pinPage( blockId );
            SlottedPage sp1( relPage );
            sp1.initBlock();
            slotNum = sp1.storeRecord( rec.getRecBytes() );
            BufferManager::getInstance(dbName)->unpinPage(blockId, true);
            recId = blockId;                    //changed
            recId = (recId<<16);
            recId = recId + slotNum;
            return recId;
        }
        BufferManager::getInstance(dbName)->unpinPage(oldBlockId, true);
        delete sp;
        sp = new SlottedPage(BufferManager::getInstance( dbName )->
                             pinPage( blockId ));
        slotNum = sp->storeRecord( rec.getRecBytes() );
    }
    BufferManager::getInstance(dbName)->unpinPage(blockId, true);
    recId = blockId;
    recId = (recId<<16);
    recId = recId + slotNum;
   /* int numAttrs = schema->getNumAttrs();
    for ( int i=0; i<numAttrs; i++ ) {
        if ( hasIndex[i] ) {
            char indexName[OBJNAME_SIZE+2];
            sprintf( indexName, "%s_%d",relName, i );
            BPlusTree bpTree( dbName, indexName, false );
            bpTree.insertEntry( rec.getInt( i ), recId );
        }
    }*/
    return recId;
}


int Relation::deleteRecord( int recId ) {
    int relId =  (recId>>16);
    char *relPage = BufferManager::getInstance( dbName )->
                    pinPage( relId );
    SlottedPage sp( relPage );
    Record rec( sp.getRecord( recId ), schema );
    int numAttrs = schema->getNumAttrs();
    for ( int i=0; i<numAttrs; i++ ) {
        if ( hasIndex[i] ) {
            char indexName[OBJNAME_SIZE+2];
            sprintf( indexName, "%s_%d",relName, i );
            BPlusTree bpTree( dbName, indexName, false );
            bpTree.deleteEntry( rec.getInt( i ) );
        }
    }
    int ret = sp.deleteRecord( recId );
    BufferManager::getInstance(dbName)->unpinPage(relId, true);

    return ret;
}

void Relation::buildIndex( int attrIndex ) {

    int attrType = ( schema )->getAttrType( attrIndex );
    if ( attrType == INT_TYPE ) {
        RIterator *itr = this->openScan();
        itr->open();

        int sysIndexId =  StorageManager::getFirstBlockId(dbName, "SYSINDEX");
        char *sysIndexPage = BufferManager::getInstance(dbName)->
                             pinPage(sysIndexId);
        SlottedPage sp(sysIndexPage);
        char sysIndexBytes[(OBJNAME_SIZE*2)+9];
        char indexName[OBJNAME_SIZE+2];
        sprintf( indexName, "%s_%d",relName, attrIndex );
        sprintf( sysIndexBytes, "%s|%s|%d", indexName, relName, attrIndex );
        sp.storeRecord( sysIndexBytes );
        BufferManager::getInstance(dbName)->unpinPage(sysIndexId, true);
        BPlusTree bpTree( dbName, indexName, true );
        hasIndex[attrIndex] = true;
        while ( true ) {
            Record rec = itr->next();
            if ( rec.getRecBytes() == NULL ) {
                break;
            }
            bpTree.insertEntry( rec.getInt( attrIndex ), itr->getCurRecId() );
        }
    } else {
        cout<<"Index can be built only on INT typed Attributes"<<endl;
    }
}

RIterator *Relation::openScan(Predicate *p) {
    return p->evaluate(this);
}

