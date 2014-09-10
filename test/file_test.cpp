#include <iostream>
#include <gtest/gtest.h>
#include <string.h>
#include "file.h"

using namespace std;
using namespace flydb;
/*
TEST(File,open)
{
	File f;

	EXPECT_EQ(f.open("test/test_open.txt"),0);
}
*/
TEST(File,addpage)
{
	File f;

	bool bexist = (access("test/test_open_get.txt",0) == 0);
	
	if(bexist){
		remove("test/test_open.txt");
	}

	EXPECT_EQ(f.open("test/test_open.txt"),0);
	
	//
	char pagedata[PAGE_SIZE]="";
	memset(pagedata,0,PAGE_SIZE);

	*(int *)&pagedata[PAGE_SIZE-sizeof(int)*2] = 0;
	*(int *)&pagedata[PAGE_SIZE-sizeof(int)] = 0;

	Page page;
	page.frombinary(pagedata);

	EXPECT_EQ(f.get_page_num(),0);

	f.addpage(page,0);
	
	EXPECT_EQ(f.get_page_num(),1);

	f.addpage(page,0);

	EXPECT_EQ(f.get_page_num(),1);
	
	f.addpage(page,1);

	EXPECT_EQ(f.get_page_num(),2);
	
	f.addpage(page,3);

	EXPECT_EQ(f.get_page_num(),4);

}

TEST(File,getpage)
{
    bool shoudcreate = (access("test/test_open_get.txt",0) != 0);
	File f;

	EXPECT_EQ(f.open("test/test_open_get.txt"),0);
	
	//
	char pagedata[PAGE_SIZE]="";
	memset(pagedata,0,PAGE_SIZE);

	*(int *)&pagedata[PAGE_SIZE-sizeof(int)*2] = 2;
	*(int *)&pagedata[PAGE_SIZE-sizeof(int)] = 0;

	Page page;
	page.frombinary(pagedata);

    if(shoudcreate){
	    f.addpage(page,0);
	    f.addpage(page,1);
	    f.addpage(page,3);
    }
	EXPECT_EQ(f.get_page_num(),4);

	Page t;

	f.getpage(t,0);

	char tdata[PAGE_SIZE];
	memset(tdata,0x33,PAGE_SIZE);
	t.tobinary(tdata);
	EXPECT_EQ( *(int *)&pagedata[PAGE_SIZE-sizeof(int)*2],2);
	EXPECT_EQ( *(int *)&pagedata[PAGE_SIZE-sizeof(int)],0);
	
	f.getpage(t,1);
	t.tobinary(tdata);
	EXPECT_EQ( *(int *)&pagedata[PAGE_SIZE-sizeof(int)*2],2);
	EXPECT_EQ( *(int *)&pagedata[PAGE_SIZE-sizeof(int)],0);
	
	f.getpage(t,2);
	t.tobinary(tdata);
	EXPECT_EQ( *(int *)&pagedata[PAGE_SIZE-sizeof(int)*2],2);
	EXPECT_EQ( *(int *)&pagedata[PAGE_SIZE-sizeof(int)],0);

}

