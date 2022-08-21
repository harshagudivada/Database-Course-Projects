/** ----------------------------------------------------
 * CS3010: Introduction to Database Systems
 * Department of Computer Science and Engineering
 * IIT Hyderabad
 *
 * File: sputil.c
 * Implementation of helper functions for the slotted page.
 * To be provided by the students.
 * -----------------------------------------------------
 */

#include<iostream>
#include<cstring>
#include<cstdlib>
#include<cstdio>

#include "sputil.h"

using namespace std;

// Returns length of a record.  Assume records are NULL 
// terminated (not true in practice, but a shortcut).
inline int length(char *record) {
    return strlen(record);
}

// Returns 2 bytes starting at block + offset as a short.
short readShort(char *block, short offset) {
    char* temp = block + offset;
    short value;
    short* shortPtr = &value;
    if(strlen[temp] < SIZE_SHORT)       //is temp ended by \0?
    {
        return -1;
    }
    else
    {
        memcpy(shortPtr,temp,SIZE_SHORT);
        return value;
    }
}

// Writes a 2 byte short value at block + offset
void writeShort(char *block, short offset, short value) {  //change the file to .cpp
    short* shortPtr = &value;
    try{
        char* temp = block + offset;
        if(strlen[temp] < SIZE_SHORT)
        {
            throw exception();
        }
        else{
            memcpy(block + offset,shortPtr,SIZE_INT);
        }
    }
        catch(exception &){
		     cout << "Exception raised:";
		     cout<<" Writing short int failed due to insufficient space "<<endl;
	    } 
}

// Returns a fragment of the byte array
char *readBytes(char *block, short recOffset, short recLength) {
    
}

// Writes a fragment in the byte array
void writeBytes(char *block, int offset, char *rec) {
    // Write your code here
}

// Move a chunk of bytes. Use memcpy to implement this.
void moveBytes(char *block, short destOffset, short sourceOffest, 
               short chunkLength) {
    // Write your code here
}
