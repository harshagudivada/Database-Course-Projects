#include "smutil.h"
int readInt(char *block, short offset) {
    int value;
    int *intPtr = &value;
    
    memcpy(intPtr,block+offset,4); 
    return value;
}

void writeInt(char *block, short offset, int value) {
    int *intPtr = &value;
    memcpy(block+offset,intPtr,4);
}

void writeBytes(char *block, short offset, char *rec) {

    memcpy(block+offset,rec,strlen(rec));
    block[offset+strlen(rec)] = '\0';
    
}


char *readBytes(char *block, short recOffset, short recLength) {
	char *rec = (char *) malloc((size_t) recLength+1);
	short endRecOffset;
	
	//scaning d record from right end
    for(short i =0;block[recOffset + i] != '\0';i++){
    		
		    rec[i] = block[recOffset + i];
			endRecOffset = i+1;	    
    }
    rec[endRecOffset] = '\0';

    return rec;
}
void moveBytes(char *offsetInSrcLeaf, 
				char *offsetInDestLeaf, 
				short chunkLength){
   
    memmove( offsetInDestLeaf, offsetInSrcLeaf, chunkLength);
}

void makeCompact(char *diskFirstblock, 
				short destOffset, 
				short sourceOffset, 
               	short chunkLength){
               	
    memmove( diskFirstblock + destOffset, 
    			diskFirstblock + sourceOffset, 
    			chunkLength);
}

