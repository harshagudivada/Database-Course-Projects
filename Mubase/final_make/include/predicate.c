#include "rm.h"

/*base class for Operators == != > >= < <=*/
class PredOperator : public Predicate {
    protected:
        char attribute[ OBJNAME_SIZE ];//left operand of operator
        char value[ OBJNAME_SIZE ];//right operand of operator

    public:
        PredOperator( char *a, char *v ) { //constructor
            for ( int i=0; a[i] != '\0'; i++ ) {
                attribute[i] = a[i];
                attribute[i+1] = '\0';
            }
            for ( int i=0; v[i] != '\0'; i++ ) {
                value[i] = v[i];
                value[i+1] = '\0';
            }
        }
};

class Equals : public PredOperator {
    public:
        Equals( char *a, char *v ) : PredOperator( a, v ) {}//constructor
        bool evaluate( Record *rec ) { //fun evaluate
            Schema *schema = rec->getSchema();
            int no = schema->getNumAttrs();
            for ( int i = 0; i < no; i++ ) {
                if ( strcmp( schema->getAttrName( i ), attribute ) == 0 ) {//check for attribute and its type
                    if ( schema->getAttrType( i ) == INT_TYPE ) {
                        return ( atoi( value ) == rec->getInt( i ) );
                    } else if ( schema->getAttrType( i ) == FLOAT_TYPE ) {
                        return ( atof( value ) == rec->getFloat( i ) );
                    } else if ( schema->getAttrType( i ) == STRING_TYPE ) {
                        return ( strcmp( value, rec->getString( i ) ) == 0 );
                    }
                }
            }
        }//end evaluate

        RIterator *evaluate(Relation *rel) {
            int attrIndex = (rel->getSchema())->getAttIndex( attribute );
            if( rel->hasIndexOn(attrIndex) ) {
                return new BTIterator( rel, attrIndex, value );
            } else {
                return new PredIterator( rel->getDbName(), rel->getRelName(), this );
            }
        }
};

class EqualsExtends : public PredOperator {
    public:
        EqualsExtends( char *a, char *v ) : PredOperator( a, v ) {}//constructor
        bool evaluate( Record *rec ) { //fun evaluate
            Schema *schema = rec->getSchema();
            int no = schema->getNumAttrs();
            for ( int i = 0; i < no; i++ ) {
                if ( strcmp( schema->getAttrName( i ), attribute ) == 0 ) {//check for attribute and its type
                    for ( int j=0; j<no; j++ ) {
                        if ( strcmp( schema->getAttrName( j ), value ) == 0 ) {

                            if ( schema->getAttrType(i) == schema->getAttrType(j) ) {
                                if ( schema->getAttrType( i ) == INT_TYPE ) {
                                    return ( rec->getInt( j ) == rec->getInt( i ) );
                                } else if ( schema->getAttrType( i ) == FLOAT_TYPE ) {
                                    return ( rec->getFloat( j ) == rec->getFloat( i ) );
                                } else if ( schema->getAttrType( i ) == STRING_TYPE ) {
                                    return ( strcmp( rec->getString( j ), rec->getString( i ) ) == 0 );
                                }
                            }
                        }
                    }
                }
            }
        }//end evaluate

        RIterator *evaluate(Relation *rel) {
            return new PredIterator( rel->getDbName(), rel->getRelName(), this );
        }
};

class Less : public PredOperator {
    public:
        Less( char *a, char *v ) : PredOperator( a, v ) {}//constructor
        bool evaluate( Record *rec ) { //fun evaluate
            Schema *schema = rec->getSchema();
            int no = schema->getNumAttrs();
            for ( int i = 0; i < no; i++ ) {
                if ( strcmp( schema->getAttrName( i ), attribute ) == 0 ) {//check for attribute and its type
                    if ( schema->getAttrType( i ) == INT_TYPE ) {
                        return ( atoi( value ) > rec->getInt( i ) );
                    } else if ( schema->getAttrType( i ) == FLOAT_TYPE ) {
                        return ( atof( value ) > rec->getFloat( i ) );
                    } else if ( schema->getAttrType( i ) == STRING_TYPE ) {
                        return ( strcmp( value, rec->getString( i ) ) > 0 );
                    }
                }
            }
        }//end evaluate

        RIterator *evaluate(Relation *rel) {
            return new PredIterator( rel->getDbName(), rel->getRelName(), this );
        }
};

class LessExtends : public PredOperator {
    public:
        LessExtends( char *a, char *v ) : PredOperator( a, v ) {}//constructor
        bool evaluate( Record *rec ) { //fun evaluate
            Schema *schema = rec->getSchema();
            int no = schema->getNumAttrs();
            for ( int i = 0; i < no; i++ ) {
                if ( strcmp( schema->getAttrName( i ), attribute ) == 0 ) {//check for attribute and its type
                    for ( int j=0; j<no; j++ ) {
                        if ( strcmp( schema->getAttrName( j ), value ) == 0 ) {

                            if ( schema->getAttrType(i) == schema->getAttrType(j) ) {
                                if ( schema->getAttrType( i ) == INT_TYPE ) {
                                    return ( rec->getInt( j ) > rec->getInt( i ) );
                                } else if ( schema->getAttrType( i ) == FLOAT_TYPE ) {
                                    return ( rec->getFloat( j ) > rec->getFloat( i ) );
                                } else if ( schema->getAttrType( i ) == STRING_TYPE ) {
                                    return ( strcmp( rec->getString( j ), rec->getString( i ) ) > 0 );
                                }
                            }
                        }
                    }
                }
            }
        }//end evaluate

        RIterator *evaluate(Relation *rel) {
            return new PredIterator( rel->getDbName(), rel->getRelName(), this );
        }
};

class LessEqual : public PredOperator {
    public:
        LessEqual( char *a, char *v ) : PredOperator( a, v ) {}//constructor
        bool evaluate( Record *rec ) { //fun evaluate
            Schema *schema = rec->getSchema();
            int no = schema->getNumAttrs();
            for ( int i = 0; i < no; i++ ) {
                if ( strcmp( schema->getAttrName( i ), attribute ) == 0 ) {//check for attribute and its type
                    if ( schema->getAttrType( i ) == INT_TYPE ) {
                        return ( atoi( value ) >= rec->getInt( i ) );
                    } else if ( schema->getAttrType( i ) == FLOAT_TYPE ) {
                        return ( atof( value ) >= rec->getFloat( i ) );
                    } else if ( schema->getAttrType( i ) == STRING_TYPE ) {
                        return ( strcmp( value, rec->getString( i ) ) >= 0 );
                    }
                }
            }
        }//end evaluate

        RIterator *evaluate(Relation *rel) {
            return new PredIterator( rel->getDbName(), rel->getRelName(), this );
        }
};

class LessEqualExtends : public PredOperator {
    public:
        LessEqualExtends( char *a, char *v ) : PredOperator( a, v ) {}//constructor
        bool evaluate( Record *rec ) { //fun evaluate
            Schema *schema = rec->getSchema();
            int no = schema->getNumAttrs();
            for ( int i = 0; i < no; i++ ) {
                if ( strcmp( schema->getAttrName( i ), attribute ) == 0 ) {//check for attribute and its type
                    for ( int j=0; j<no; j++ ) {
                        if ( strcmp( schema->getAttrName( j ), value ) == 0 ) {
                            if ( schema->getAttrType(i) == schema->getAttrType(j) ) {
                                if ( schema->getAttrType( i ) == INT_TYPE ) {
                                    return ( rec->getInt( j ) >= rec->getInt( i ) );
                                } else if ( schema->getAttrType( i ) == FLOAT_TYPE ) {
                                    return ( rec->getFloat( j ) >= rec->getFloat( i ) );
                                } else if ( schema->getAttrType( i ) == STRING_TYPE ) {
                                    return ( strcmp( rec->getString( j ), rec->getString( i ) ) >= 0 );
                                }
                            }
                        }
                    }
                }
            }
        }//end evaluate

        RIterator *evaluate(Relation *rel) {
            return new PredIterator( rel->getDbName(), rel->getRelName(), this );
        }
};

class Greater : public PredOperator {
    public:
        Greater( char *a, char *v ) : PredOperator( a, v ) {}//constructor
        bool evaluate( Record *rec ) { //fun evaluate
            Schema *schema = rec->getSchema();
            int no = schema->getNumAttrs();
            for ( int i = 0; i < no; i++ ) {
                if ( strcmp( schema->getAttrName( i ), attribute ) == 0 ) {//check for attribute and its type
                    if ( schema->getAttrType( i ) == INT_TYPE ) {
                        return ( atoi( value ) < rec->getInt( i ) );
                    } else if ( schema->getAttrType( i ) == FLOAT_TYPE ) {
                        return ( atof( value ) < rec->getFloat( i ) );
                    } else if ( schema->getAttrType( i ) == STRING_TYPE ) {
                        return ( strcmp( value, rec->getString( i ) ) < 0 );
                    }
                }
            }
        }//end evaluate

        RIterator *evaluate(Relation *rel) {
            return new PredIterator( rel->getDbName(), rel->getRelName(), this );
        }
};

class GreaterExtends : public PredOperator {
    public:
        GreaterExtends( char *a, char *v ) : PredOperator( a, v ) {}//constructor
        bool evaluate( Record *rec ) { //fun evaluate
            Schema *schema = rec->getSchema();
            int no = schema->getNumAttrs();
            for ( int i = 0; i < no; i++ ) {
                if ( strcmp( schema->getAttrName( i ), attribute ) == 0 ) {//check for attribute and its type
                    for ( int j=0; j<no; j++ ) {
                        if ( strcmp( schema->getAttrName( j ), value ) == 0 ) {
                            if ( schema->getAttrType(i) == schema->getAttrType(j) ) {
                                if ( schema->getAttrType( i ) == INT_TYPE ) {
                                    return ( rec->getInt( j ) < rec->getInt( i ) );
                                } else if ( schema->getAttrType( i ) == FLOAT_TYPE ) {
                                    return ( rec->getFloat( j ) < rec->getFloat( i ) );
                                } else if ( schema->getAttrType( i ) == STRING_TYPE ) {
                                    return ( strcmp( rec->getString( j ), rec->getString( i ) ) < 0 );
                                }
                            }
                        }
                    }
                }
            }
        }//end evaluate

        RIterator *evaluate(Relation *rel) {
            return new PredIterator( rel->getDbName(), rel->getRelName(), this );
        }
};

class GreaterEqual : public PredOperator {
    public:
        GreaterEqual( char *a, char *v ) : PredOperator( a, v ) {}//constructor
        bool evaluate( Record *rec ) { //fun evaluate
            Schema *schema = rec->getSchema();
            int no = schema->getNumAttrs();
            for ( int i = 0; i < no; i++ ) {
                if ( strcmp( schema->getAttrName( i ), attribute ) == 0 ) {//check for attribute and its type
                    if ( schema->getAttrType( i ) == INT_TYPE ) {
                        return ( atoi( value ) <= rec->getInt( i ) );
                    } else if ( schema->getAttrType( i ) == FLOAT_TYPE ) {
                        return ( atof( value ) <= rec->getFloat( i ) );
                    } else if ( schema->getAttrType( i ) == STRING_TYPE ) {
                        return ( strcmp( value, rec->getString( i ) ) <= 0 );
                    }
                }
            }
        }//end evaluate

        RIterator *evaluate(Relation *rel) {
            return new PredIterator( rel->getDbName(), rel->getRelName(), this );
        }
};

class GreaterEqualExtends : public PredOperator {
    public:
        GreaterEqualExtends( char *a, char *v ) : PredOperator( a, v ) {}//constructor
        bool evaluate( Record *rec ) { //fun evaluate
            Schema *schema = rec->getSchema();
            int no = schema->getNumAttrs();
            for ( int i = 0; i < no; i++ ) {
                if ( strcmp( schema->getAttrName( i ), attribute ) == 0 ) {//check for attribute and its type
                    for ( int j=0; j<no; j++ ) {
                        if ( strcmp( schema->getAttrName( j ), value ) == 0 ) {
                            if ( schema->getAttrType(i) == schema->getAttrType(j) ) {
                                if ( schema->getAttrType( i ) == INT_TYPE ) {
                                    return ( rec->getInt( j ) <= rec->getInt( i ) );
                                } else if ( schema->getAttrType( i ) == FLOAT_TYPE ) {
                                    return ( rec->getFloat( j ) <= rec->getFloat( i ) );
                                } else if ( schema->getAttrType( i ) == STRING_TYPE ) {
                                    return ( strcmp( rec->getString( j ), rec->getString( i ) ) <= 0 );
                                }
                            }
                        }
                    }
                }
            }
        }//end evaluate

        RIterator *evaluate(Relation *rel) {
            return new PredIterator( rel->getDbName(), rel->getRelName(), this );
        }
};

/*base connectors like and, or, not */
class PredConnector : public Predicate {
    protected:
        Predicate *leftSubExp;//left expression
        Predicate *rightSubExp;//right expression

    public:
        PredConnector( Predicate *l, Predicate *r )
            :leftSubExp(l),rightSubExp(r)	{}
};

//and operator class
class And : public PredConnector {
    public:
        And ( Predicate *l, Predicate *r ) :PredConnector( l, r ) {}//constructor
        bool evaluate( Record *rec ) {
            bool left = leftSubExp->evaluate( rec );//solve left exp
            bool right = rightSubExp->evaluate( rec );//solve right exp
            return ( left && right );
        }//end evaluate

        RIterator *evaluate(Relation *rel) {
            return new PredIterator( rel->getDbName(), rel->getRelName(), this );
        }
};

//or operator class
class Or : public PredConnector {
    public:
        Or ( Predicate *l, Predicate *r ) :PredConnector( l, r ) {}//constructor
        bool evaluate( Record *rec ) {
            bool left = leftSubExp->evaluate( rec );//solve left exp
            bool right = rightSubExp->evaluate( rec );//solve right exp
            return ( left || right );
        }//end evaluate

        RIterator *evaluate(Relation *rel) {
            return new PredIterator( rel->getDbName(), rel->getRelName(), this );
        }
};

//not operator class
class Not : public Predicate {
    private:
        Predicate *subExp;

    public:
        Not ( Predicate *l ) :subExp(l) {}
        bool evaluate( Record *rec ) {
            bool original = subExp->evaluate( rec );//solve exp
            return ( !original );
        }

        RIterator *evaluate(Relation *rel) {
            return new PredIterator( rel->getDbName(), rel->getRelName(), this );
        }
};
