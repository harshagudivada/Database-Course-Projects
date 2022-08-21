#include<iostream>			//Header files
#include<cstdlib>
#include<sstream>
#include<fstream>
#include<cstring>
#include<cctype>
using namespace std;
inline int fn()				//global function to maintain a count of files created,inline to avoid linker errors
{
	static int fileNo=1;
	return fileNo++;
}
class Relation;				//pre declaration of friend classes
class Schema;
class CrossProduct;
class Projection;
class Rename;
class Predicate;
class attrib				//class attrib
{
friend class Projection;		//friend function declarations
friend class Schema;
friend class Relation;
friend class CrossProduct;
friend class Rename;
friend class Predicate;
friend ostream &operator<<(ostream&,Schema&);
friend ostream &operator<<(ostream&,Relation&);
public:
	attrib(string n,int t)
		:name(n),type(t)
	{
	}
	attrib(){;}
private:
	string name;				//name and type of the attribute 
	int type;
	
};
class Schema					//Class Schema
{
friend class Predicate;
friend class Rename;
friend class Projection;
friend class Relation;
friend ostream &operator<<(ostream&,Relation&);
friend ostream &operator<<(ostream&,Schema&);
friend class CrossProduct;
public:
	Schema()
	{;
	}
	Schema(const Schema &sch)			//copy constructor
	{
		atrib_no = sch.atrib_no;
		atrib_arr = new attrib [atrib_no];
		for(int i=0;i < atrib_no;i++)
		{
			atrib_arr[i] = sch.atrib_arr[i];
		}
	}
	bool operator==(const Schema & sch)		//==operator overloading for Schema
	{
		if( atrib_no == sch.atrib_no)
		{
			for(int i=0;i < atrib_no;i++)
			{
				if( (atrib_arr[i].type != sch.atrib_arr[i].type) )  //only type is checked
				return false;
			}
			return true;
		}
		else return false;
	} 
private:
	int atrib_no;
	attrib* atrib_arr;
};

inline ostream &operator<<(ostream& out,Schema& sch)		//<<operator overloaded for Schema
{
	for(int i=0;i<sch.atrib_no;i++)
	{
		out<<sch.atrib_arr[i].name<<":";
		switch(sch.atrib_arr[i].type)
		{
			case 0:
			out<<"int";
			break;
			
			case 1:
			out<<"float";
			break;
			
			case 2:
			out<<"string";
			break;
		}	
		if( i == (sch.atrib_no - 1)) 
		{
			break; 
		}
		out<<"|";
	}
	return out;
}
class Record					//class Record
{
friend class Predicate;
friend class Projection;
friend class Relation;
friend ostream &operator<<(ostream&,Relation&);
friend ostream &operator<<(ostream&,Record&);
friend istream &operator>>(istream&,Record&);
public:
	Record(int inp_size)			//constructor
		:arr_size(inp_size)
	{
		st = new string [inp_size];
		type_arr = new int [inp_size];
	}
	Record(const Record &rec)		//copy constructor
		:arr_size(rec.arr_size)
	{
		st = new string[arr_size];
		type_arr = new int [arr_size];
		for(int i=0;i<arr_size;i++)
		{
			st[i] = rec.st[i];
			type_arr[i] = rec.type_arr[i];
		}
	}
	bool operator==(const Record & rec)				//==operator overloading for Record
	{
		if(arr_size != rec.arr_size) return false;
		int il=0,ir=0;float fl=0.0,fr=0.0;string sl="";string sr="";
		for(int i=0; i<arr_size;i++)
		{
			switch(type_arr[i])
			{
				case 0:
				il = atoi((st[i]).c_str()); ir = atoi((rec.st[i]).c_str());
				if( il != ir) return false;
				break;
				
				case 1:
				fl = atof((st[i]).c_str()); fr = atof((rec.st[i]).c_str());
				if( fl != fr) return false;
				break;
				
				case 2:
				if(st[i] != rec.st[i]) return false;
				break;
			}
		}
		return true;
	}
private:
	int arr_size;
	string* st;			//Each record has array of strings
	int* type_arr;			// 0 - int , 1 - float , 2 - string,this array is used in comparing two records
};
inline ostream &operator<<(ostream& out,Record& rec) // this ostream is a super class for fstream
{							// So, it works for both cout to console or output to a file.
	for(int i=0;i < rec.arr_size;)
	{
		out << rec.st[i];
		i++;
		if(i!=rec.arr_size) 
		out <<"|";
	}
	return out;
}

class Relation				//class Relation
{
friend ostream &operator<<(ostream&, Relation&);
public:
	Relation(string r,bool mode)
		:relName(r)
	{
		string  fname = relName+".dat";
		if(mode == 0)				//Checks for mode, i.e., relation created can be foe a existing file or a new file
		{
			inOut.open(&fname[0],ios::in | ios::out);
			if(!inOut)
			{
				cout<<"File coulldn't be opened for creating the relation\n";
				exit(1);
			}
		}
		else
		{
			inOut.open(&fname[0],ios::out);
			if(!inOut)
			{
				cout<<"New file coulldn't be opened for creating the relation\n";
				exit(1);
			}
		}
	}
	void setSchema()				//setSchema function
	{
		string s1="";string atrib_name="";
		string type="";string temp="";
		int c=0,count=0,index=0,atrib_type;
		inOut.seekg(0);
		getline(inOut,s1);
		if(!inOut)
		{
			cout<<"File coulldn't be opened for setting schema\n";
			exit(1);
		}
		int len  = s1.length();
		while(c<len)
		{
			if(s1[c] == ':') count++;
			c++;
		}
		sch.atrib_no = count;
		sch.atrib_arr = new attrib[count];
		c=0;
		while(c < len)
		{
			while( s1[c] != ':')
			{
				atrib_name+=s1[c];		//creating attribute names in the schema
				c++;
			}
			c++;
			while( s1[c] != '|' && c < len)
			{
				type+=s1[c];			//creating attribute types in the Schema
				c++;
			}
			c++;
			if( type == "int") atrib_type = 0;
			if( type == "float") atrib_type = 1;
			if( type == "string") atrib_type = 2;
			attrib a1(atrib_name,atrib_type);
			sch.atrib_arr[index] = a1;index++;
			atrib_name.clear();type.clear();
		}
	}
	Schema* getSchema()					//getSchema function returns the Schema of a relation
	{
		Schema* schPtr = new Schema();
		schPtr -> atrib_no = sch.atrib_no;
		schPtr->atrib_arr = new attrib[sch.atrib_no];
		for(int i=0;i < sch.atrib_no;i++)
		{
			schPtr->atrib_arr[i] = sch.atrib_arr[i];
		}
		return schPtr;
	}
	void addRecord(Record rec)			//add Record function
	{
		if(!inOut)
		{
			cout<<"File coulldn't be opened for adding record\n";
			exit(1);
		}
		inOut << rec <<endl;
	}
	Record* getNextRecord()				//getNextRecord function returns the next record from the relation
	{
		if(!inOut)
		{
			cout<<"File coulldn't be opened for reading record\n";
			exit(1);
		}
		Record* recPtr = new Record(sch.atrib_no);
		string line="";string temp=""; int len=0,count=0;
		getline(inOut,line);
		if( !inOut ) return NULL;
		while(line.find_first_not_of("\t\f\v\r\n ") == string::npos)		//check for any non blank characterrs,if none is  
		{								//present,it means it is a blnak/empty line(space is important)
			line.clear();			
			getline(inOut,line);				//skip the blank line and get another line,useful in cross product
			if(inOut.eof()){ return NULL;}
		}
		len = line.length();
		for(int i=0;i <len;)
		{
			if( line[i] == '|')				//split the line wiht | as delimiter into columns
			{
				recPtr->st[count] = temp;recPtr->type_arr[count] = sch.atrib_arr[count].type;
				count++;
				temp.clear();
				i++;
				continue;
			}
			temp+=line[i];i++;
		}
		recPtr->st[count] = temp;recPtr->type_arr[count] = sch.atrib_arr[count].type;
		return recPtr;
	}
	void open(string fn)						//open function used to open file corresponding to the relation
	{
		inOut.close();
		inOut.open(&fn[0],ios::out | ios::in);
		if(!inOut)
		{
			cout<<"File "<<fn<<" coulldn't be opened\n";
			exit(1);
		}
		string temp = fn;
		relName = fn.erase(fn.find("."));			//extract the relation name from the file name by removing the .dat
	}
	void close()							//close function used to close an opened file, useful in switching between  
	{								//reading and writing modes
		inOut.clear();
		inOut.close();
		if(!inOut)
		{
			cout<<"File "<<(relName)+".dat"<<" couldn't be closed\n";
			exit(1);
		}
	}

	Schema sch;
	fstream inOut;
	string relName;
	
};
inline ostream &operator<<(ostream &output, Relation &rel)		//==opeartor overloaded for Relation class
{
	cout<<(rel.sch)<<endl;
	Record* temp;
	rel.inOut.seekg(0); string st="";
	getline(rel.inOut,st);
	while((temp = rel.getNextRecord()) != NULL)
	output <<(*temp)<<endl;
	return output;
	 
}

