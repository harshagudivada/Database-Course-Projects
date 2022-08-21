/** ----------------------------------------------------
 * CS3010: Introduction to Database Systems
 * Department of Computer Science and Engineering
 * IIT Hyderabad
 *
 * File: spimpl.cpp
 * Implementation of the methods provided by class SlottedPage.
 * Implementation to be provided by students. 
 * -----------------------------------------------------
 */

#include<iostream>
#include<cstring>
#include<cstdlib>
#include<cstdio>

#include "global.h"
#include "sp.h"
#include "smutil.h"

using namespace std;

/**
* Initializes (formats) the slotted page
*/
void SlottedPage::initBlock() {
    writeShort(block, (short)0, (short)0);					// no.of records in the block
    writeShort(block, (short)2, (short)(BLOCKSIZE-4));		// End of free space
	writeShort(block, (short)(BLOCKSIZE-2),-1);				// Block id of next block
	writeShort(block, (short)(BLOCKSIZE-4),0);				// no.of free slots in slot array
	writeShort(block, (short)4, (short)-1);					// offset of first free slot in slotarray
}

/**
 * Shows the contents of the page
 */
void SlottedPage::display() {
    short n    = readShort(block, (short) 0);
    short eofs = readShort(block, (short) 2);
	short mideofs = readShort(block,(BLOCK_SIZE-4));

    short availableSpace = eofs - (n + 1)*4 - SIZE_SHORT;	// Available space
 
    cout << "Number of slot entries = " << n << endl
         << "Available free space = " << availableSpace << endl;
	
	// Display the record corresponding to each slot
    for (short i = 1; i <= n+mideofs; i++) {
		cout << "Record at slot " << i << endl;
		char *rec = getRecord(i);

		// If free slot display none
		if (rec == NULL) 
		   	cout << "None"<<endl; 
		else
		    cout << rec<<endl;
    }
    cout << endl;
}

/**
 * Get record given a slot number
 */
char *SlottedPage::getRecord(short slotNum) {
	char *reqrec;
	short loc,recsize;
	short n = readShort(block, (short)0);
	short mideofs = readShort(block,BLOCK_SIZE-4);

	// If slot number is lessthan 0 or greater than the size of slot array return NULL
	if( (n+mideofs)<slotNum || slotNum <= 0 )
		return NULL;
	else{
		recsize = readShort(block, (3*SIZE_SHORT)+(slotNum-1)*2*SIZE_SHORT);
		// If the given slot number is of free slot, return \0
		if (recsize == -1)
		return "\0";
		loc = readShort(block, (3*SIZE_SHORT)+(slotNum-1)*2*SIZE_SHORT+SIZE_SHORT);
		reqrec = readBytes(block, loc, recsize);
		return reqrec;
	}
}

/**
 * Store a record and return slot number of stored record
 */
short SlottedPage::storeRecord(char *record) {
    short reclength = strlen(record);								// record length
	reclength++;													// increment reclength, because we store \0 also
	short numrec = readShort(block, (short)0);						// no.of records in present block
	short num_mideof=readShort(block,(BLOCK_SIZE-4));				// no.of free slots in slot array
	short firstmideof = readShort(block, (short)4);					// offset of first free slot
	short temp=firstmideof;
	short eofs = readShort(block, (short) 2);						// offset of end of file
	short availspace = 0;											// available space

	if(num_mideof == 0)
		availspace = (eofs - (numrec + 1)*4 - SIZE_SHORT) - 2*SIZE_SHORT;
	else
		availspace = (eofs - (numrec + 1)*4 - SIZE_SHORT) - (num_mideof - 1)*4;

	// available space is less than required space
	if(availspace < reclength)
		return -1;
	// sufficient available space
	else{
		writeBytes(block,eofs-reclength,record);		// write the record at the end of free space
		// no free slots in slot array
		if(num_mideof == 0){
			writeShort(block,3*SIZE_SHORT+numrec*2*SIZE_SHORT,reclength);					// store length at the end of slot array
			writeShort(block,3*SIZE_SHORT+numrec*2*SIZE_SHORT+SIZE_SHORT,eofs-reclength);	// store offset at the end of slot array
			writeShort(block,(short)0,numrec+1);											// update no.of records in block
			writeShort(block,(short)2,eofs-reclength);										// update end of free space
			return numrec+1;																// return slot number of the stored record
		}
		else{
			temp = firstmideof;													// slot number of first free slot 
			writeShort(block, (short)4,readShort(block,temp+SIZE_SHORT));		// update the offset of first free slot
			writeShort(block,temp,reclength);									// store record length in the slot array
			writeShort(block,temp+SIZE_SHORT,eofs-reclength);					// store offset in the slot array
			writeShort(block,(short)0,numrec+1);								// update no.of records in block
			writeShort(block,(short)2,eofs-reclength);							// update end of free space
			writeShort(block,(short)BLOCK_SIZE-4,num_mideof-1);					// update the no.of free slots
			return ( (firstmideof-3*SIZE_SHORT)/(2*SIZE_SHORT) + 1 );			// return slot number of the stored record
		}
		
	}
}

/**
 * Delete a record given its slot number and returns the
 * space available.
 */
short SlottedPage::deleteRecord(short slotNum) {

	short numrec = readShort(block, (short)0);			// no.of records in the block before delete
	short num_mideof=readShort(block,(BLOCK_SIZE-4));	// no.of free slots before delete

	// if given slot  number is greater than slot array size return -1
	if(slotNum > numrec+num_mideof)
		return -1;

	short firstmideof = readShort(block, (short)4);								//offset of first free slot
	short eofs = readShort(block, (short) 2);									// end of free space
	short reclength = readShort(block, 3*SIZE_SHORT+(slotNum-1)*2*SIZE_SHORT);	// size of record to be deleted

	// if given slot is free slot return -1
	if( reclength == -1 )
		return -1;

	short reclocation = readShort(block, 3*SIZE_SHORT+(slotNum-1)*2*SIZE_SHORT+SIZE_SHORT);	// offset of the record to be deleted
	
	// insert the slot of record to be deleted at the beginning of free slot list
	writeShort(block,(short)4,3*SIZE_SHORT+(slotNum-1)*2*SIZE_SHORT);				// present slot as the first free slot in the free slot list
	writeShort(block,3*SIZE_SHORT+(slotNum-1)*2*SIZE_SHORT+SIZE_SHORT,firstmideof);	// link it with previous freee list
	writeShort(block,3*SIZE_SHORT+(slotNum-1)*2*SIZE_SHORT,-1);						// store offset as -1
	
	makeCompact(block,eofs+reclength,eofs,reclocation-eofs);	// compact so that free space is at one place

	// After compact update the slot array with present locations of the records
	short templocation;
	short tempreclength;	
	for(int i=1;i<=numrec+num_mideof;i++){
		tempreclength = readShort(block,3*SIZE_SHORT+(i-1)*2*SIZE_SHORT);
		if(tempreclength != -1){
			templocation = readShort(block,3*SIZE_SHORT+(i-1)*2*SIZE_SHORT+SIZE_SHORT);
			// updation of locations
			if(templocation < reclocation){
				writeShort(block,3*SIZE_SHORT+(i-1)*2*SIZE_SHORT+SIZE_SHORT,templocation+reclength);
			} 
		}
		
	}

	writeShort(block,(short)SIZE_SHORT,eofs+reclength);		// Update end of free space
	writeShort(block,(short)0,numrec-1);					// Update no.of records
	writeShort(block, (short)(BLOCK_SIZE-4), num_mideof+1);	// Update no.of free slots
	return eofs;	
}

/**
 * To get the blockid of next block
 */
short SlottedPage::getNextBlockId()
{
	return (readShort(block,(BLOCK_SIZE-2)));
}

/**
 * To store the blockid of next block in the present block
 */
void SlottedPage::setNextBlockId(short nextid)
{
	writeShort(block,(BLOCK_SIZE-2),nextid)	;
}

/**
 * The sum of occupied and unoccupied places in slot array
 */
int SlottedPage::totalSlots()
{
	short n    = readShort(block, (short) 0);
	short mideofs = readShort(block,(BLOCK_SIZE-4));
	return (n + mideofs);
}
