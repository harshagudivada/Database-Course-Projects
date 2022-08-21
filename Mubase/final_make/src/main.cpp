#include"sort.h"
#include<iostream>
using namespace std;

int main()
{	
	Schema s;
	//s.setSchema( "c_id:int|c_name:string|c_salary:float|c_id2:int|c_name2:string|c_salary2:float" );
	s.setSchema( "l_orderkey:int|l_shipmode:string|l_orderkey2:int|l_shipmode2:string" );

	char *dbName = "MyDb";
	DiskManager::createDB( dbName ,500);
	StorageManager::initDB(dbName);
	StorageManager::createObject( dbName, "SYSOBJECT" );
	StorageManager::createObject( dbName, "SYSCOLUMN" );
	StorageManager::createObject( dbName, "SYSINDEX" );
	
	int sysObjectId =  StorageManager::getFirstBlockId(dbName, "SYSOBJECT");
    char *sysObjPage = BufferManager::getInstance(dbName)->
                                        pinPage(sysObjectId);  
    SlottedPage sp(sysObjPage);
    sp.initBlock();
    BufferManager::getInstance(dbName)->unpinPage(sysObjectId, true);
        
    int sysColId =  StorageManager::getFirstBlockId(dbName, "SYSCOLUMN");
    char *sysColPage = BufferManager::getInstance(dbName)->
                                        pinPage(sysColId);
    SlottedPage sp1(sysColPage);
    sp1.initBlock();
	BufferManager::getInstance(dbName)->unpinPage(sysColId, true);
	
	int sysIndexId =  StorageManager::getFirstBlockId(dbName, "SYSINDEX");
    char *sysIndexPage = BufferManager::getInstance(dbName)->
                                        pinPage(sysIndexId);
    SlottedPage sp2(sysIndexPage);
    sp2.initBlock();
	BufferManager::getInstance(dbName)->unpinPage(sysIndexId, true);

	//cout<<"Before creating relation---------------------------------------------------\n";
	Relation r("MyDb", "MyRel", true, &s);
	//cout<<"After creating relation----------------------------------------------------\n";
	//Record* recPtr = NULL;
	//stringstream ss;
	//string temp;
	//float value;

	Record *rec;
    	for (int i=0; i<10000; i++) {
		char recBytes[200];
		cin.getline( recBytes, 200 );
		rec = new Record(recBytes, &s);
		r.addRecord( *rec );
		delete rec;
	}

	//srand(time(0));
	/*for(int i=0;i<200;i++){
		//cout<<"i= "<<i<<endl;
    	value = ((float)rand() * 10000.0)/(float)RAND_MAX ;		
		ss.clear();
		temp.clear();
		ss << value;
		ss >> temp;
		temp = "1|root|"+temp+"|0|root|10.10";
		//cout<<"temp= "<<temp<<endl;
		recPtr = new Record (&temp[0], &s);
		//cout<<"here1\n";
		r.addRecord( *recPtr );
	}*/

	/*Relation r("MyDb", "MyRel", true, &s);
	Record rec("900|root|100.00|0|root|10.10", &s);
	r.addRecord( rec );
	Record rec1("500|admin|50.00|0|root|10.02", &s);
	int recId = r.addRecord( rec1 );
	Record rec2("700|vijay|25.00|0|root|10.1", &s);
	r.addRecord( rec2 );
	*/


	//cout<<"Before creating sort object----------------------------------------------------\n";
    Sort s1(dbName,"MyRel","l_shipmode");
	//cout<<"After creating sort object\n";
	s1.open();
	//cout<<"After open---------------------------------------------------------------------------------------------------------------\n";
	Record rec3 = s1.next();	
	while(rec3.getRecBytes() != NULL){          //do it till null  
		cout<<rec3.getRecBytes()<<endl;
		rec3 = s1.next();
		cin.get();
    }
    
	return 0;
}
