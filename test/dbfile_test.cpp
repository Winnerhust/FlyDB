#include <iostream>
#include <gtest/gtest.h>

#include "dbfile.h"

using namespace std;
using namespace flydb;

TEST(DBFile,load)
{
	char filepath[30] = "test/record_test.ini";
	SchemaMgr ini;
	ini.open(filepath);

	EXPECT_EQ(ini.hasschema("Scores"),true);

    Schema *schema = ini.getschema("Scores");
	DBFile dbfile;

	bool bexist = (access("test/dbfile.db",0) == 0);
	
	if(bexist){
		remove("test/dbfile.db");
	}

	dbfile.open("test/dbfile.db");

	dbfile.load("test/load_data.txt",*schema);
	
	EXPECT_EQ(dbfile.writepage_->get_num_record(),4);

	dbfile.movetofirst();

	Record rec;

	while(dbfile.getnext(rec) != -1){
		rec.print(schema);
	}

}

TEST(DBFile,getnext)
{
	char filepath[30] = "test/record_test.ini";
	SchemaMgr ini;
	ini.open(filepath);

	EXPECT_EQ(ini.hasschema("Scores"),true);

    Schema *schema = ini.getschema("Scores");
	DBFile dbfile;


	dbfile.open("test/dbfile2.db");

	dbfile.movetofirst();

	Record rec;

	while(dbfile.getnext(rec) != -1){
		rec.print(schema);
	}

}
