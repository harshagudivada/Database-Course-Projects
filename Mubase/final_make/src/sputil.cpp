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
	short value;
	short *shortPtr = &value;

	memcpy(shortPtr,block+offset,2);
	return value;
}

// Writes a 2 byte short value at block + offset
void writeShort(char *block, short offset, short value) {
    short *shortPtr = &value;
	memcpy(block+offset,shortPtr,2);
}

// Move a chunk of bytes. Use memcpy to implement this.
void moveBytes(char *block, short destOffset, short sourceOffset, 
               short chunkLength) {
	memmove(  block+destOffset, block + sourceOffset, chunkLength);
}

