#include "rm.h"

Record::Record(char *recBytes, Schema *schema) {
	this->recBytes = recBytes;
	this->schema = schema;
	//this->hold = new char[OBJNAME_SIZE];
}

Record::Record(){
	this->recBytes = NULL;
	this->schema = NULL;
}
//attrIndex starts from ZERO
int Record::getInt(int attrIndex) {
	if ( schema->getAttrType(attrIndex) == INT_TYPE ) {
		char temp[OBJNAME_SIZE];
		for ( int i=0, count=0; recBytes[i] != '\0'; i++ ) {
			for( int j=0; recBytes[i] != '|' && recBytes[i] != '\0'; j++  ){
				temp[j] = recBytes[i];
				temp[j+1] = '\0';
				i++;
			}
			if ( recBytes[i] == '|' || attrIndex == 0 ) {
				if ( attrIndex == count ) {
					return atoi(temp);
				}
				count++;
			}
		}
		return atoi(temp);
	}
	else
		cout<<"Type doesn't match\n";
}

float Record::getFloat(int attrIndex) {
	if ( schema->getAttrType(attrIndex) == FLOAT_TYPE ) {
		char temp[OBJNAME_SIZE];
		for ( int i=0, count=0; recBytes[i] != '\0'; i++ ) {
			for( int j=0; recBytes[i] != '|' && recBytes[i] != '\0'; j++  ){
				temp[j] = recBytes[i];
				temp[j+1] = '\0';
				i++;
			}
			if ( recBytes[i] == '|' || attrIndex == 0 ) {
				if ( attrIndex == count ) {
					return atof(temp);
				}
				count++;
			}
		}
		return atof(temp);
	}
	else
		cout<<"Type doesn't match\n";
}

char *Record::getString( int attrIndex ) {
	if ( schema->getAttrType(attrIndex) == STRING_TYPE ) {
		char *hold = new char[OBJNAME_SIZE];
		for ( int i=0, count=0; recBytes[i] != '\0'; i++ ) {
			for( int j=0; recBytes[i] != '|' && recBytes[i] != '\0'; j++  ){
				hold[j] = recBytes[i];
				hold[j+1] = '\0';
				i++;
			}
			if ( recBytes[i] == '|' || attrIndex == 0 ) {
				if ( attrIndex == count ) {
					return hold;
				}
				count++;
			}
		}
		return hold;
	}
	else
		cout<<"Type doesn't match\n";
}

char *Record::getRecBytes() {
	return this->recBytes;
}

bool Record::equals( Record right,  int attrIndex )	{	// equality comparison between two records
		if ( schema->getAttrType( attrIndex ) == INT_TYPE ) {
			if ( getInt(attrIndex) != right.getInt(attrIndex) ){	// comparing integers
				return false;
			}
		}else if ( schema->getAttrType( attrIndex ) == FLOAT_TYPE  ) {
			if ( getFloat(attrIndex) != right.getFloat(attrIndex) ){	// comparing float type
			 	return false;
			}
		}else if ( schema->getAttrType( attrIndex ) == STRING_TYPE ) {
			if ( strcmp( getString(attrIndex), right.getString(attrIndex) ) != 0 ) {		// comparing strings
				return false;
			}
		}
		return true;
}

bool Record::less( Record right,  int attrIndex )	{
		if ( schema->getAttrType( attrIndex ) == INT_TYPE ) {
			if ( getInt(attrIndex) >= right.getInt(attrIndex) ){
				return false;
			}
		}else if ( schema->getAttrType( attrIndex ) == FLOAT_TYPE  ) {
			if ( getFloat(attrIndex) >= right.getFloat(attrIndex) ){
			 	return false;
			}
		}else if ( schema->getAttrType( attrIndex ) == STRING_TYPE ) {
			if ( strcmp( getString(attrIndex), right.getString(attrIndex) ) >= 0 ) {
				return false;
			}
		}
		return true;
}

bool Record::greater( Record right,  int attrIndex )	{
		if ( schema->getAttrType( attrIndex ) == INT_TYPE ) {
			if ( getInt(attrIndex) <= right.getInt(attrIndex) ){
				return false;
			}
		}else if ( schema->getAttrType( attrIndex ) == FLOAT_TYPE  ) {
			if ( getFloat(attrIndex) <= right.getFloat(attrIndex) ){
			 	return false;
			}
		}else if ( schema->getAttrType( attrIndex ) == STRING_TYPE ) {
			if ( strcmp( getString(attrIndex), right.getString(attrIndex) ) <= 0 ) {
				return false;
			}
		}
		return true;
}
