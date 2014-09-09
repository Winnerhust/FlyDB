#include <iostream>
#include <string>
#include <string.h>
#include <gtest/gtest.h>
#include "record.h"

using namespace std;
using namespace flydb;

TEST(Record,print)
{
    //填充一条Record记录
    char recordbuf[200];

    memset(recordbuf,0,sizeof(recordbuf));

    int *index = ((int *)recordbuf);
    index[0]=sizeof(int)*5;//int
    index[1]=sizeof(int)*(5+1);//string
    index[2]=sizeof(int)*(5+1);//double
    index[3]=sizeof(int)*(5+1)+sizeof(double);//string
    index[4]=sizeof(int)*(5+1)+sizeof(double)+19;//string
    
    int *studentid = (int *) &(recordbuf[index[0]]);
    *studentid = 2005;
    
    double *scores = (double *) &(recordbuf[index[2]]);
    *scores = 94.5;

    strncpy(&(recordbuf[index[3]]),"'2014-9-7 16:02:24'",index[4]-index[3]);
    
    //加载Schema
	char filepath[30] = "test/record_test.ini";
	SchemaMgr ini;
	ini.open(filepath);

	EXPECT_EQ(ini.hasschema("Scores"),true);

    Schema *schema = ini.getschema("Scores");
    
    ini.print();

    //打印该记录
    Record record;

    record.copybits(recordbuf,sizeof(recordbuf));

    record.print(schema);

}

TEST(Record,suck)
{
    //写入数据文件
    char content[]="2005||94.5|'2014-09-07 22:29:10'|";
    char suckfilepath[] = "test/suck.txt";
    FILE *fp = fopen(suckfilepath,"w");

	fwrite(content,1,sizeof(content),fp);
	fclose(fp);

    //加载Schema
	char filepath[30] = "test/record_test.ini";
	SchemaMgr ini;
	ini.open(filepath);

	EXPECT_EQ(ini.hasschema("Scores"),true);

    Schema *schema = ini.getschema("Scores");
    
    //
    fp = fopen(suckfilepath,"r");
    
    Record record;

    record.suck(schema,fp);
    
    record.print(schema);

    fclose(fp);

}

TEST(Record,project)
{
    //填充一条Record记录
    char recordbuf[200];

    memset(recordbuf,0,sizeof(recordbuf));

    int *index = ((int *)recordbuf);
    index[0]=sizeof(int)*5;//int
    index[1]=sizeof(int)*(5+1);//string
    index[2]=sizeof(int)*(5+1);//double
    index[3]=sizeof(int)*(5+1)+sizeof(double);//string
    index[4]=sizeof(int)*(5+1)+sizeof(double)+19;//string
    
    int *studentid = (int *) &(recordbuf[index[0]]);
    *studentid = 2005;
    
    double *scores = (double *) &(recordbuf[index[2]]);
    *scores = 94.5;

    strncpy(&(recordbuf[index[3]]),"'2014-9-7 16:02:24'",index[4]-index[3]);

    //加载投影后的Schema
	char filepath[30] = "test/record_test.ini";
	SchemaMgr ini;
	ini.open(filepath);

	EXPECT_EQ(ini.hasschema("NewScores"),true);

    Schema *schema = ini.getschema("NewScores");
    
    ini.print();
    
    //投影
    Record record;
    record.copybits(recordbuf,sizeof(recordbuf));
    
    int attrs_keep[]={ 0,1,3 };

    record.project(attrs_keep,3);

    record.print(schema);

}

TEST(Record,marge)
{
    //填充一条Record记录
    char recordbuf[200];

    memset(recordbuf,0,sizeof(recordbuf));

    int *index = ((int *)recordbuf);
    index[0]=sizeof(int)*5;//int
    index[1]=sizeof(int)*(5+1);//string
    index[2]=sizeof(int)*(5+1);//double
    index[3]=sizeof(int)*(5+1)+sizeof(double);//string
    index[4]=sizeof(int)*(5+1)+sizeof(double)+19;//string
    
    int *studentid = (int *) &(recordbuf[index[0]]);
    *studentid = 2005;
    
    double *scores = (double *) &(recordbuf[index[2]]);
    *scores = 94.5;

    strncpy(&(recordbuf[index[3]]),"'2014-9-7 16:02:24'",index[4]-index[3]);

    //加载投影后的Schema
	char filepath[30] = "test/record_test.ini";
	SchemaMgr ini;
	ini.open(filepath);

	EXPECT_EQ(ini.hasschema("MargeScores"),true);

    Schema *schema = ini.getschema("MargeScores");
    
    ini.print();
    
    //投影
    Record record,lrecord,rrecord;
    lrecord.copybits(recordbuf,sizeof(recordbuf));
    rrecord.copybits(recordbuf,sizeof(recordbuf));
    
    int attrs_keep[]={ 0,1,3 };
    lrecord.project(attrs_keep,3);


    int lattrs_keep[]={ 0,1,2 };
    int rattrs_keep[]={ 0,1,2,3 };
    
    record.marge(lrecord,rrecord,lattrs_keep,3,rattrs_keep,4);

    record.print(schema);

}
