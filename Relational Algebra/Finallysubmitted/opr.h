#include"rel.h"				//include all schema,record,relation classes declarations
class basicPredicate			//basic predicate class (abstract class)
{
public:
	basicPredicate()
	{
	}
	virtual bool evaluate(Record r, Schema s ) = 0;		//virtua function evaluate
};
class RAop						//Basr class RAop for all operator classes
{
public:
	RAop(string inp_str)
		:str(inp_str)
	{
	}
	RAop(){;}					//default constructor;
	virtual Relation*  evaluate() = 0;
	virtual	Schema* getSchema() = 0;		//virtual functions
	virtual bool valid_schema() = 0;
private:
	string str;
};
class BaseRelop :public RAop
{
public:
	BaseRelop(string r)
	{
		relPtr = new Relation( r, 0);		//BseRelop creates a relation fr a existing file,so mode 0
		relPtr->setSchema();			//setting the schema of the relation
	} 
	virtual bool valid_schema()			
	{
		return true;
	}
	virtual Schema* getSchema()
	{
		return relPtr->getSchema();
	}
	virtual Relation* evaluate()
	{
		return relPtr;
	}
private:
Relation* relPtr;
};
class BinaryRAop: public RAop 				//BinaryRAOP class derived from RAop
{
public:
	BinaryRAop(RAop &l,RAop &r)			//Constructor initialising both subexpressions of BinaryRAop
		:leftSubExp(&l),rightSubExp(&r)
		{
		}
	
	virtual Schema* getSchema()				
	{
		if(valid_schema() == true)
		{
			return (leval->getSchema());
		}
		else
		{
			cout<<"Invalid schema\n";exit(1);
		}
	}
	virtual bool valid_schema()
	{
		Schema* lsch;Schema* rsch;
		lsch = leval->getSchema();
		rsch = reval->getSchema();
		if( (*lsch) == (*rsch) ) return true;		//call overloaded == operator for Schema class
		else return false;
	}
	
protected:					//protected so that inherited classes can access tham
RAop* leftSubExp;
RAop* rightSubExp;
Relation* reval;
Relation* leval;
};


class Minus:public BinaryRAop			//Minus class(set difference)
{
public:
	Minus(RAop &l,RAop &r)
		:BinaryRAop(l,r)
	{
	}

	virtual Relation*  evaluate()			//evaluate function
	{
		string str="";string rname="";stringstream ss;
		Record* lrec;Record* rrec;
		Relation* tmp;
		leval = leftSubExp->evaluate();		//first evaluate left and right subexpressions
		reval = rightSubExp->evaluate();
		if( valid_schema() == true)		//check if their schemas are correct
		{	
			ss << fn();
			ss >> rname;		
			tmp = new Relation (rname,1);	//create a temporary file with a number as a file name
			ss.clear();
			leval->inOut.clear();reval->inOut.clear();	//As returned files from left and right expressions are opened in writing mode
			leval->close();reval->close();			//close them and open in read/write mode
			leval->open(leval->relName+".dat");reval->open(reval->relName+".dat");
			leval->inOut.seekg(0);
			getline( leval->inOut,str); 				//skip Schema
			tmp->sch = ( *( getSchema() ) );			
			(tmp->inOut) << (tmp->sch) <<endl;			//output Schema to file
			while( (lrec=leval->getNextRecord()) !=NULL)		//loop through all records of both files
			{
				reval->inOut.clear();
				reval->inOut.seekg(0);
				getline( reval->inOut,str);
				while( (rrec=reval->getNextRecord()) != NULL)	// if records are equal then skip that record 
				{
					if( (*lrec) == (*rrec) ) break;
				}
				if( rrec != NULL ) continue;
				else tmp->addRecord(*lrec);			// else output that record to file (This will be the set diffrrence)
			}
		}
		else 
		{
			cout<<"Invalid schema\n";
			exit(1);
		}
		return tmp;
	}
};

class Union:public BinaryRAop				//class Union
{
public:
	Union(RAop &l,RAop &r)
		:BinaryRAop(l,r)
	{
	}
	Relation* evaluate()				//virtua function overwritten
	{
		string str="";string rname="";stringstream ss;
		Record* lrec;Record* rrec;
		Relation* tmp;
		leval = leftSubExp->evaluate();		//first evaluate left and right subexpressions
		reval = rightSubExp->evaluate();
		if( valid_schema() == true)
		{	
			ss << fn();
			ss >> rname;
			tmp = new Relation (rname,1);
			ss.clear();
			leval->inOut.clear();reval->inOut.clear();
			leval->close();reval->close();
			leval->open(leval->relName+".dat");reval->open(reval->relName+".dat");	//the first part calculates the set difference i.e.,
			leval->inOut.seekg(0);							// AUB = (A-B)+B
			getline( leval->inOut,str); 
			tmp->sch = ( *( getSchema() ) );			
			(tmp->inOut) << (tmp->sch) <<endl;
			while( (lrec=leval->getNextRecord()) !=NULL)
			{
				reval->inOut.clear();
				reval->inOut.seekg(0);
				getline( reval->inOut,str);
				while( (rrec=reval->getNextRecord()) != NULL)
				{
					if( (*lrec) == (*rrec) ) break;
				}
				if( rrec != NULL ) continue;
				else tmp->addRecord(*lrec);
			}
		}
		else 
		{
			cout<<"Invalid schema\n";
			exit(1);
		}
		string st ="";
		(reval->inOut).clear();
		reval->inOut.seekg(0);
		getline(reval->inOut,st);
		Record* recPtr=NULL;
		while( (recPtr = reval->getNextRecord()) != NULL)		//Second part copies the records the right expression to the output file
		{
			tmp->addRecord(*recPtr);
		}
		return tmp;
	}
private:
};

class CrossProduct: public BinaryRAop			//class CrossProduct
{
public:
	CrossProduct(RAop& l,RAop& r)
		:BinaryRAop(l,r)
	{
	}
	virtual bool valid_schema()
	{
		return true;
	}
	virtual Schema* getSchema()			//getSchema function
	{
		if(valid_schema() == true)
		{
			Schema* lsch = leval->getSchema();		//gets the Schema of both the relations
			Schema* rsch = reval->getSchema();
			Schema* sch = new Schema();
			sch->atrib_no = lsch->atrib_no + rsch->atrib_no;
			sch->atrib_arr = new attrib [sch->atrib_no];
			string s1;string s2;
			for(int i=0;i<(lsch->atrib_no);i++)		//creates the new Schema by appending .to each new attribute wiht its relation
			{
				(sch->atrib_arr[i]).name = (leval->relName)+"."+( (lsch->atrib_arr[i]).name );	//say in AXB, new attributes will be 
				(sch->atrib_arr[i]).type = ( (lsch->atrib_arr[i]).type );		//A.x,A.y and B.z,B.v etc.
			}
			for(int i=(lsch->atrib_no),j=0;i<(sch->atrib_no);i++,j++)
			{
				(sch->atrib_arr[i]).name = (reval->relName)+"."+( (rsch->atrib_arr[j]).name );
				(sch->atrib_arr[i]).type = ( (rsch->atrib_arr[j]).type );
			}
			return sch;
		}
		else {
			cout<<"Invalid schema\n";exit(1);
		}
	}
	Relation* evaluate()			//evaluate function of CrossProduct
	{
		 leval = leftSubExp->evaluate();		//first evaluate left and right subexpressions
		 reval = rightSubExp->evaluate();
		if(valid_schema() == true)	
		{
			Record* lrec;Record* rrec;string str="";stringstream ss;string rname="";
			ss << fn();
			ss >> rname;
			Relation* tmp = new Relation (rname,1);
			leval->inOut.clear();reval->inOut.clear();
			leval->close();reval->close();
			leval->open(leval->relName+".dat");reval->open(reval->relName+".dat");
			leval->inOut.seekg(0);
			getline( leval->inOut,str); 
			tmp->sch = ( *( getSchema() ) );		
			(tmp->inOut) << (tmp->sch) <<endl;			
			while( (lrec = leval->getNextRecord()) != NULL)		//loop through all the records of boht relations 
			{							
				reval->inOut.clear();
				reval->inOut.seekg(0);
				getline( reval->inOut,str);
				while( (rrec = reval->getNextRecord()) != NULL)		//create new records by appending two records
				{
					(tmp->inOut)<<(*lrec)<<"|"<<(*rrec)<<endl;
				}
			}
			return tmp;
		}
		else
		{
			cout<<"Invalid schema\n";exit(1);
		}
		
	}
private:
};

class UnaryRAop: public RAop				//UnaryOp derived from RAop (Abstract class)
{
public:
	UnaryRAop(RAop &e)
		:subExp(&e)
	{
	}
	virtual bool valid_schema()
	{
		return true;
	}
	virtual Schema* getSchema()		//should be overloaded by projection,Selection and rename classes
	{
		return subExp->getSchema();
	}
	virtual Relation* evaluate() = 0;
	
protected:
RAop* subExp;
Relation* eval;
};

class Projection :public UnaryRAop		//Class Projection
{
public:
	Projection(RAop &e,string s)
		:UnaryRAop(e),str(s)
	{
	}
	virtual Schema* getSchema()		//getSchema 
	{
		Schema* esch = eval->getSchema();
		Schema* sch = new Schema();
		int len = str.length(),c=0;
		if(len == 0) 
		{
			cout<<"No attributes in Schema\n";	//If no attributes in the Schema, exit with error
			exit(1) ;
		}
		else sch->atrib_no = 1;
		string temp="";
		while(c<len)
		{
			if(str[c] == ',') (sch->atrib_no)++;c++;	//loop to find no.of attributes in the predicate
		}
		sch->atrib_arr = new attrib[sch->atrib_no];
		iarr = new int [sch->atrib_no];
		int sch_no=0,i=0;
		for(int c=0;c<len;)
		{
			if(str[c] == ',')			//extract the attributes from the predicate
			{
				for(i=0;i<esch->atrib_no;i++)
				{
					if(temp == esch->atrib_arr[i].name)
					{
						sch->atrib_arr[sch_no].name = temp;
						sch->atrib_arr[sch_no].type = esch->atrib_arr[i].type;iarr[sch_no] = i; (sch_no)++; temp.clear();break;
					}
				}
				if( i == esch->atrib_no)
				{
					cout<<"Given attributes are not present in the input schema\n"; exit(1);
				}
				
			}
			else
			{
				temp+=str[c];
			}
			c++;
		}
		for(i=0;i<esch->atrib_no;i++)				//create Schema from the extracted attributes
		{
			if(temp == esch->atrib_arr[i].name)
			{					
				sch->atrib_arr[sch_no].name = temp;
				sch->atrib_arr[sch_no].type = esch->atrib_arr[i].type;iarr[sch_no] = i; temp.clear();break;
			}
		}
		if( i == esch->atrib_no)
		{
			cout<<"Given attributes are not present in te input schema\n"; exit(1);
		}
		return sch;
	}
	Relation* evaluate()			//evaluate function
	{
		eval = subExp->evaluate();	//first evaluate the subexpression
		Record* recPtr; stringstream ss;string rname="";string st;
		ss << fn();
		ss >> rname;
		Relation* tmp = new Relation (rname,1);
		(tmp->sch) = (*(getSchema()));			//setting schema calls getschema which also validates schema
		(tmp->inOut) << (tmp->sch) <<endl;
		eval->inOut.clear();
		eval->close();
		eval->open(eval->relName+".dat");
		eval->inOut.seekg(0);
		getline(eval->inOut,st);
		while( (recPtr = eval->getNextRecord()) !=NULL)		//create a temporary file which can have duplicate records 
		{	
			for(int i=0;i<(tmp->sch).atrib_no;)
			{
				(tmp->inOut) << recPtr->st[iarr[i]];i++;
				if(i != (tmp->sch).atrib_no) (tmp->inOut)<<"|";
				else (tmp->inOut)<<endl;
			}
		}
		tmp->inOut.close();tmp->open(tmp->relName+".dat");getline(tmp->inOut,st); 
		Record* dup;Record* dist;
		ss.clear();rname.clear();
		ss << fn();
		ss >> rname;
		Relation* final = new Relation (rname,1);		// now create another file with only distinct records
		final->sch = tmp->sch;
		(final->inOut) << (tmp->sch) <<endl; int record=0,c=0;
		while( (dup = tmp->getNextRecord()) !=NULL)		//take a record and check for its duplicate, if present don't include it
		{
			record++;
			while( (dist = tmp->getNextRecord()) !=NULL)
			{
				if( (*dup) == (*dist) )
				{
					break;
				}
			}
			if(dist == NULL)
			{
				(final->inOut) << (*dup) << endl;	//if not present, write it to output
			}
			tmp->inOut.clear();
			tmp->inOut.close();
			tmp->open((tmp->relName)+".dat");
			tmp->inOut.seekg(0);
			getline(tmp->inOut,st);c=0;
			while(c<record){ tmp->getNextRecord();c++;}
		}
		return final;
	}
	
	
private:
string str;
int* iarr;
};


class Rename: public UnaryRAop			//Rename class inherited from UnaryOp class
{
public:
	Rename(RAop &e,string r,string nat)
		:UnaryRAop(e),rname(r),natrib(nat)		//Constructor takes new name and new attribute names
	{
	}
	Schema* getSchema()					//return Schema 
	{
		Schema* esch = eval->getSchema();
		if(natrib!="")
		{
			int c=0,i=0;string temp="";
			Schema* sch = new Schema();
			sch->atrib_no = esch->atrib_no;
			sch->atrib_arr = new attrib [sch->atrib_no];
			int len = natrib.length();
			while( c <len )				//loop through the predicate and extract new Schema
			{
				if(natrib[c] == ',')
				{
					sch->atrib_arr[i].name = temp;
					sch->atrib_arr[i].type = esch->atrib_arr[i].type;
					temp.clear();i++; 
				}
				else temp+=natrib[c];
				c++;
			}
			sch->atrib_arr[i].name = temp;
			sch->atrib_arr[i].type = esch->atrib_arr[i].type;
			if( i != (sch->atrib_no)-1 )
			{
				cout<<"Insufficient no.of attributes\n";exit(1);
			}
			return sch;
		}
		else return esch;
	}
	Relation* evaluate()						//evaluate function
	{
		eval = subExp->evaluate();				//evaluate subpression
		(eval->inOut).clear();
		eval->close();
		eval->open(eval->relName+".dat");
		(eval->inOut).seekg(0);
		Relation* tmp = new Relation(rname,1);
		(tmp->sch) = (*(getSchema()));
		(tmp->inOut) << (tmp->sch) << endl;
		Record* recPtr;string str="";
		getline(eval->inOut,str);
		while( (recPtr = eval->getNextRecord()) != NULL)	//loop through all records and output them to file
		{
			(tmp->inOut) << (*recPtr) << endl;		
		}	
		return tmp;
	}
private:
	string rname;
	string natrib;
};

class Select: public UnaryRAop				//Select class
{
public:
	Select(RAop &e,basicPredicate* p)		//Constructor takes RAop object and a pointer to the root of predicate tree
		:prePtr(p),UnaryRAop(e)
	{
	}
	Schema* getSchema()
	{
		Schema* sch = eval->getSchema();
		return sch;
	}
	virtual Relation* evaluate()			//evaluate function
	{
		eval = subExp->evaluate();		//evaluate subpression
		eval->inOut.clear();
		eval->close();
		eval->open(eval->relName+".dat");
		eval->inOut.seekg(0);
		stringstream ss;string temp; 
		ss << fn();
		ss >> temp;
		Relation* tmp = new Relation(temp,1);
		(tmp->sch) = (*(getSchema()));
		(tmp->inOut) << (tmp->sch) << endl;
		Record* recPtr;string str="";
		getline(eval->inOut,str);
		while( (recPtr = eval->getNextRecord()) != NULL)		//loop through all the records
		{
			if( (prePtr->evaluate((*recPtr),(tmp->sch))) == true )	//call evaluate on the predicate tree root, if it returs true
			{
				(tmp->inOut) << (*recPtr) << endl;		//include the record in the output
			}
		}
		return tmp;	
		
	} 
private:
	basicPredicate* prePtr;					//pointer to the predicate tree
	
};
/*int main()
{
	BaseRelop A("A");
	BaseRelop B("B");
	Rename re1(A,"new","roll_no,fees,city");
	//Projection p1(A,"branch,balance,account_number");
	//CrossProduct cp1(A,B);
	//Minus m1(A,B);
	//Union u1(A,B);
	//Relation* r1 = m1.evaluate();
	//Relation* r1 = u1.evaluate_union();
	//Relation* r1 = cp1.evaluate();
	//cout<<"here1\n";
	Relation* r1 = re1.evaluate();
	r1->close();
	r1->open("new.dat");
	cout<<(*r1)<<endl;
}*/
//#endif
