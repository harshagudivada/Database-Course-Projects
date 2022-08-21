#include "rm.h"

char *Schema::getSchema () {
	//maximum size of type is 6
	char schema[ ( numAttrs*OBJNAME_SIZE ) + ( numAttrs * 6 ) ];
	int count = 0;
	for ( int i=0; i < numAttrs; i++ ) {
		for( int j=0; attrName[i][j] != '\0'; j++ ) {
			schema[count] = attrName[i][j];
			count++;
		}
		schema[count] = ':';
		count++;
		switch( attrType[i] ){
			case INT_TYPE :{
				char *temp = "int";
				for( int j=0; temp[j] != '\0'; j++ ){
					schema[count] = temp[j];
					count++;
				}
				break;
			}
			case FLOAT_TYPE :{
				char *temp = "float";
				for( int j=0; temp[j] != '\0'; j++ ){
					schema[count] = temp[j];
					count++;
				}
				break;
			}
			case STRING_TYPE :{
				char *temp = "string";
				for( int j=0; temp[j] != '\0'; j++ ){
					schema[count] = temp[j];
					count++;
				}
				break;
			}
		}
		schema[count] = '|';
		count++;
	}
	schema[count-1] = '\0';
	cout<<schema<<endl;
	return schema;
}

void Schema::setSchema ( char *schema ) {
	int index=0;
	char *type = new char[OBJNAME_SIZE];
	for ( int i=0, j=0; schema[i] != '\0'; i++ ) {
		if ( schema[i] == ':' ) {
			type[j] = '\0';
			j = 0;
			attrName[index] = type;
			type = new char[OBJNAME_SIZE];
		}
		else if ( schema[i] == '|' ) {
			type[j] = '\0';
			j = 0;
			attrType[index] = stringToType(type);
			type = new char[OBJNAME_SIZE];
			index++;
		}
		else {
			type[j] = schema[i];
	 		j++;
	 	}
	}
	attrType[index] = stringToType(type);
	numAttrs = index + 1;
}

int Schema::stringToType ( char *type ) {
 	switch(type[0]) {
 		case 'i' :
 			return INT_TYPE;
 		case 'f' :
 			return FLOAT_TYPE;
 		case 's' :
 			return STRING_TYPE;
 		default :
 			return -1;
 	}
 }

int Schema::getAttrType( int index ){
	return attrType[index];
}

char *Schema::getAttrName( int index ){
	return attrName[index];
}

int Schema::getNumAttrs() {
	return numAttrs;
}

//given an attribute return its index
int Schema::getAttIndex( char *attriName ) {
    for ( int i=0; i < numAttrs; i++ ) {
        if ( strcmp( attrName[i], attriName ) == 0 ) {
            return i;
        }
    }
    return -1;
}
