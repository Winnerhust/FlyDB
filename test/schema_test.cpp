#include <iostream>
#include <gtest/gtest.h>
#include "stringutil.h"
#include "schema.h"

using namespace std;
using namespace stringutil;
using namespace flydb;
string raw_content="[sys_table]\n"
"col_info=TABLE_ID,Int\n"
"col_info=COLUMNS,Int\n"
"col_info=TABLE_NAME,String\n"
"\n"
"#comment\n"
"[Student]\n"
"col_info=ID,Int\n"
" col_info=NAME ,  String\n"
"  col_info= SEX ,INT\n"
"col_info=GRADE_ID,INT\n"
"\n"
"#SCORES\n"
"[Scores]\n"
"col_info=StudentID,INT\n"
"col_info=COURSE,string\n"
"col_info=SCORES,double\n"
"col_info=TIME,string\n";

TEST(stringutil,trim)
{
	string buf = "   aaa    ";
	trim(buf);

	EXPECT_EQ(buf,string("aaa"));

	buf =  "   aaa";
	trim(buf);
	EXPECT_EQ(buf,string("aaa"));

	buf =  "aaa    ";
	trim(buf);
	EXPECT_EQ(buf,string("aaa"));
}

TEST(stringutil,trimleft)
{
	string p = "   aaa    ";
	trimleft(p);

	EXPECT_EQ(p,string("aaa    "));

	p =  "   aaa";
	trimleft(p);
	EXPECT_EQ(p,string("aaa"));

	p = "aaa    ";
	trimleft(p);
	EXPECT_EQ(p,string("aaa    "));
}

TEST(stringutil,trimright)
{
	string p = "   aaa    ";
	trimright(p);
	
	EXPECT_EQ(p,string("   aaa"));

	p =  "   aaa";
	trimright(p);
	EXPECT_EQ(p,string("   aaa"));

	p = "aaa    ";
	trimright(p);
	EXPECT_EQ(p,string("aaa"));
}

TEST(stringutil,getline)
{
	//create a new ini file
	char filepath[] = "test/test.ini";
	FILE *fp = fopen(filepath,"w+");
	char content[]=" USER=root \r\n [COMMON] \n DB=sys   	\nPASSWD=tt   \n#commit   \n ;--------- \n[DEFINE] \nname=cxy\n";
	fwrite(content,1,sizeof(content),fp);
	fclose(fp);

	//test
	
	string line;
	fp = fopen(filepath,"r");

	getline(line,fp);
	trimright(line,'\n');
	trimright(line,'\r');
	trim(line);
	EXPECT_EQ(line,"USER=root");
	
	getline(line,fp);
	trimright(line,'\n');
	trimright(line,'\r');
	trim(line);
	EXPECT_EQ(line,"[COMMON]");
	
	getline(line,fp);
	trimright(line,'\n');
	trimright(line,'\r');
	trim(line);
	EXPECT_EQ(line,"DB=sys");
	
	getline(line,fp);
	getline(line,fp);
	trimright(line,'\n');
	trimright(line,'\r');
	trim(line);
	EXPECT_EQ(line,"#commit");
	fclose(fp);
}

////////////////////////////

TEST(SchemaMgr,pasre)
{
	SchemaMgr section;
	string s = "DB=sys";
	string key,value;

	EXPECT_EQ(section.parse(s.c_str(),key,value),true);
	EXPECT_EQ(key,"DB");
	EXPECT_EQ(value,"sys");
	
	s= "DB";
	key = value = "";
	
	EXPECT_EQ(section.parse(s.c_str(),key,value),false);
	EXPECT_EQ(key,"");
	EXPECT_EQ(value,"");

	s= "DB=";
	key = value = "";
	
	EXPECT_EQ(section.parse(s.c_str(),key,value),true);
	EXPECT_EQ(key,"DB");
	EXPECT_EQ(value,"");

	s= "=sys";
	key = value = "";
	
	EXPECT_EQ(section.parse(s.c_str(),key,value),true);
	EXPECT_EQ(key,"");
	EXPECT_EQ(value,"sys");
}


TEST(SchemaMgr,hasschema)
{
	//create a new ini file
	char filepath[] = "test/test.ini";
	FILE *fp = fopen(filepath,"w+");
	char content[1024]="";
    strcpy(content,raw_content.c_str());

	fwrite(content,1,sizeof(content),fp);
	fclose(fp);

	//test
	SchemaMgr ini;
	ini.open(filepath);
	cout<<"---------------\n"<<endl;
	ini.print();
	cout<<"---------------\n"<<endl;
	
	EXPECT_EQ(ini.hasschema("sys_table"),true);
	EXPECT_EQ(ini.hasschema("Student"),true);
	EXPECT_EQ(ini.hasschema("Scores"),true);
	EXPECT_EQ(ini.hasschema("SS"),false);

}



#ifdef GTEST_MAIN
int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#endif
