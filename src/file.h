#ifndef _FLYDB_FILE_H
#define _FLYDB_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "page.h"
using namespace std;
//#define PAGE_SIZE (1024)
namespace flydb {
/*
class Page
{
public:	
	Page(long page_size = PAGE_SIZE):page_size_(page_size),bits_(NULL){}
	~Page(){ free(bits_);}

	void set_page_size(long page_size){page_size_ = page_size;}
	long get_page_size(){return page_size_ ;}

	void tobinary(char *bits){ strncpy(bits,bits_,page_size_);}
	void frombinary(char *bits){free(bits_); bits_ = strndup(bits,page_size_);}
private:
	long page_size_;
	char *bits_;
};
*/
class File
{
public:
	File();
	~File();
	
	long get_page_num() const{return page_num_;}
	long get_page_size() const{return page_size_;}

	int open(const char *fname);
	void close();

	int getpage(Page &page,long pageno);
	void addpage(Page &page,long pageno);
private:
	FILE *fp_;
	long page_num_;
	long page_size_;
};



}//end of namespace


#endif
