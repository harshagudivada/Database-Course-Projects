//#include "rm.h"
#include "predicate.c"

BaseRelIterator::BaseRelIterator(const char *dbName, const char *relName) {	
	this->dbName = dbName;
	this->relName = relName;
	this->curBlockBuff = NULL;
	this->sp = NULL;
	this->curSlot = 1;
	Relation r( this->dbName, this->relName );
	this->schema = r.getSchema();
}

Record BaseRelIterator::next() {
	char *recBytes = sp->getRecord( curSlot );
	while ( recBytes != NULL && recBytes[0] == '\0' ) {
		curSlot++;
		recBytes = sp->getRecord(curSlot);
	}
	
	if ( recBytes == NULL ) {
		int blockId = sp->getNextBlockId();
		
		if ( blockId == -1 ) {
			//cout<<"EOF of relation"<<endl;
			Record rec ( NULL, schema );
			delete sp;							//modified 
			BufferManager::getInstance(dbName)->unpinPage(curBlock, true);
			return rec;
		}
		delete sp;
		BufferManager::getInstance(dbName)->unpinPage(curBlock, true);
		curBlock = blockId;
        	curBlockBuff = BufferManager::getInstance(dbName)->
                                        pinPage(blockId);
                sp = new SlottedPage(curBlockBuff);
                curSlot = 1;
                recBytes = sp->getRecord( curSlot );
                while ( recBytes != NULL && recBytes[0] == '\0' ) {
			curSlot++;
			recBytes = sp->getRecord(curSlot);
		}
	}
	
	Record rec ( recBytes, schema );
	curSlot++;
	return rec;
}

void BaseRelIterator::close() {
	BufferManager::getInstance(dbName)->unpinPage(curBlock, true);
}
