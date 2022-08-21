#include"opr.h"					//include all Operator classes definitions
class Predicates: public basicPredicate		//class Predicates derived from Base Predicate class(Abstract class)
{
public:
	Predicates(string s)
		:con(s)
	{
	}
	virtual bool evaluate(Record,Schema) = 0;	
protected:
string con;
};

class Predicate: public basicPredicate			//class Predicate
{
public:
	Predicate(string l,string o,string r,int t)	//constructor takes left opnd,right opnd,operator,type of right opnd
		:lopnd(l),opr(o),ropnd(r),type(t)
	{
	}
	bool evaluate(Record rec,Schema sch)		//evaluate fn
	{
		int il=0,ir=0,i=0;float fl=0.0,fr=0.0;string sl="";string sr="";int ltype=0,rtype=0;stringstream ss;
		for(i=0;i<sch.atrib_no;i++)		//look for the type and name of left operand 
		{
			if(sch.atrib_arr[i].name == lopnd)
			{
				switch(sch.atrib_arr[i].type)
				{
					case 0:
					il = atoi((rec.st[i]).c_str()); ltype = 0;break;	//assign types 	0 - integer
					case 1:								//	1 - float
					fl = atof((rec.st[i]).c_str());  ltype = 1;break;		//	2 - string
					case 2:
					sl = rec.st[i]; ltype = 2;break;
				}
				break;
			}
		}
		if( i == sch.atrib_no) {cout<<"lopnd ="<<lopnd<<endl;cout<<"Invalid predicate1\n";exit(1);}	//if predicate attributes are not
		switch(type)					//present, return with an error
		{
			case 1:					//Based on the type of right argument, create a variable for comparison
			for(i=0;i<sch.atrib_no;i++)
			{
				if(sch.atrib_arr[i].name == ropnd)
				{
					switch(sch.atrib_arr[i].type)
					{
						case 0:				//convert the operands to their respective types
						ir = atoi((rec.st[i]).c_str()); rtype = 0;break; 
						case 1:
						fr = atof((rec.st[i]).c_str());rtype = 1;break; 
						case 2:
						sr = rec.st[i];rtype = 2;break;
					}
					break;
				}
			}
			if( i == sch.atrib_no) {cout<<"Invalid predicate2\n";exit(1);}
			if(ltype != rtype){cout<<"ltype="<<ltype<<endl;cout<<"rtype="<<rtype<<endl;cout<<"Incompatiable attributes in the predicate1\n";exit(1);}
			break;
			
			case 2:
			ss << ropnd;
			ss >> ir;ss.clear();
			break;
			
			case 3:
			ss << ropnd;
			ss >> fr;ss.clear();
			break;
			
			case 4:
			sr = ropnd;
			break;
		}
		if(ltype != (type-2) && type!=1)				//Check for wrong query i.e., giving string attibute where float is 
		{cout<<"incompatiable attributes in the predicate2\n";		//expected
		exit(1);}
		if( opr == "=")				//comparison opeartors
		{
			switch(ltype)			//Depending on the type of attribute
			{
				case 0:
				return ( il == ir);break;
				case 1:
				return ( fl == fr);break;
				case 2:
				return ( sl == sr);break;
			}
		}
		else
		{
			if(opr == ">")
			{
				switch(ltype)
				{
					case 0:
					return ( il > ir);break;
					case 1:
					return ( fl > fr);break;
					case 2:
					return ( sl > sr);break;
				}
			}
			else
			{
				if(opr == "<")
				{
					switch(ltype)
					{
						case 0:
						return ( il < ir);break;
						case 1:
						return ( fl < fr);break;
						case 2:
						return ( sl < sr);break;
					}
				}
				else
				{
					if(opr == "!=")
					{
						switch(ltype)
						{
							case 0:
							return ( il != ir);break;
							case 1:
							return ( fl != fr);break;
							case 2:
							return ( sl != sr);break;
						}
					}
					else
					{
						if(opr == ">=")
						{
							switch(ltype)
							{
								case 0:
								return ( il >= ir);break;
								case 1:
								return ( fl >= fr);break;
								case 2:
								return ( sl >= sr);break;
							}
						}
						else
						{
							switch(ltype)
							{
								case 0:
								return ( il <= ir);break;
								case 1:
								return ( fl <= fr);break;
								case 2:
								return ( sl <= sr);break;
							}
						}
					}
				}
			}
		}
	}
private:
string lopnd;
string opr;
string ropnd;
int type;
};

class unaryPredicates: public Predicates		//Unary predicates derived from Predicates class i.e., NOT (~)
{
public:unaryPredicates(basicPredicate* p,string s)
	:Predicates(s),opnd(p)
{
}
	bool evaluate(Record rec,Schema sch)
	{
		return (! (opnd->evaluate(rec,sch)) );
	}
private:
	basicPredicate* opnd;
};

class binaryPredicates: public Predicates			//BinaryPredicates derived from Predicates
{
public:
	binaryPredicates(basicPredicate* l,string s,basicPredicate* r)
		:lp(l),Predicates(s),rp(r)
	{
	}
	virtual bool evaluate(Record rec,Schema sch)		//check the connector
	{
		if( con == "^" )
		{
			cout<<"In ^\n";
			return ( (lp->evaluate(rec,sch) == true) && (rp->evaluate(rec,sch) == true) );
		}
		else
		{
			return ( (lp->evaluate(rec,sch) == true) || (rp->evaluate(rec,sch) == true) );
		}
	}
private:					// two pointers to two basicPredicate classes
	basicPredicate* lp;
	basicPredicate* rp;
};
