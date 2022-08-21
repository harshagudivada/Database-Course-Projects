#include "rm.h"

void PredIterator::open() {
    itr = new BaseRelIterator(dbName, relName);
    itr->open();
}

Record PredIterator::next() {
    while ( true ) {
        Record rec =  itr->next();
        if ( rec.getRecBytes() == NULL ) {
            break;
        }
        if ( predicate->evaluate( &rec ) ) {
            return rec;
        }
    }
    Record rec( NULL, itr->getSchema() );
    return rec;
}

void PredIterator::close() {
    itr->close();
}

BTIterator::BTIterator(Relation *rel, int attrIndex, char *value) {
    this->rel = rel;
    this->attrIndex = attrIndex;
    this->value = value;
    first = true;
}

void BTIterator::open() {
    char indexName[OBJNAME_SIZE+2];
    sprintf( indexName, "%s_%d",rel->getRelName(), attrIndex );
    bt = new BPlusTree( rel->getDbName(), indexName, false );
}

//checks only for int keys and equality
Record BTIterator::next() {
    if( first ) {
        int recId = bt->lookUp(atoi(value));
        short slot = recId;
        recId = recId>>16;
        short blockId = recId;
        char *block = BufferManager::getInstance(rel->getDbName())->pinPage(blockId);
        SlottedPage sp(block);
        Record rec(sp.getRecord(slot),rel->getSchema());
        BufferManager::getInstance(rel->getDbName())->unpinPage(blockId,false);
        first = false;
        return rec;
    }
    Record rec(NULL,NULL);
    return rec;
}

void BTIterator::close() {
    delete bt;
}
