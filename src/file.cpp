#ifndef _FLYDB_FILE_CPP
#define _FLYDB_FILE_CPP
#include <unistd.h>
#include "file.h"

namespace flydb{

File::File()
	:fp_(NULL),page_num_(0),page_size_(PAGE_SIZE)
{
}

File::~File()
{
	close();
}

int File::open(const char *fname)
{
	if(fp_ != NULL){
		fclose(fp_);
	}
	if(access(fname,0)== -1){
		fp_ = fopen(fname,"w+");
	}else{
		fp_ = fopen(fname,"rw");
	}
	
	if(fp_ == NULL){
		fprintf(stderr,"failed to open file:[%s]!\n",fname);
		return -1;
	}

	char ch = getc(fp_);

	if(ch != EOF){ // file already exists
		fseek(fp_,0,SEEK_SET);
		fread(&page_num_,sizeof(long),1,fp_);
		fread(&page_size_,sizeof(long),1,fp_);
	}else{ // file does not exist
		page_num_ = 0;
		page_size_ = PAGE_SIZE;
	}

	return 0;
}

void File::close()
{
	if(fp_ != NULL){
		fclose(fp_);
		fp_ = NULL;
	}

    page_num_ = 0;
    
    page_size_ = PAGE_SIZE;
}

int File::getpage(Page &page,long  pageno)
{
	if(pageno >= page_num_){
		fprintf(stderr,"you tried to read past the end of the file\n");
		return -1;
	}

	char *bits = new (std::nothrow) char[page_size_];

	if(bits == NULL){
		fprintf(stderr,"no enough memory!\n");
		exit(-1);
	}

	fseek(fp_,pageno*page_size_+2*sizeof(long),SEEK_SET);
	fread(bits,sizeof(char),page_size_,fp_);
	
	page.set_page_size(page_size_);

	page.frombinary(bits);
	
	delete [] bits;
	return 0;
}

void File::addpage(Page &page,long pageno)
{
	int freespace = 0;

	int record_num = 0;

	if(pageno >= page_num_ ){
		for(long i = page_num_; i < pageno; ++i){
			fseek(fp_,(i+1)*page_size_ + 2*sizeof(long) - 2*sizeof(int),SEEK_SET);
			fwrite(&record_num,sizeof(int),1,fp_);
			fwrite(&freespace,sizeof(int),1,fp_);
		}
		
		page_num_ = pageno + 1;
	}

	char *bits = new (std::nothrow) char[page_size_];

	if(bits == NULL){
		fprintf(stderr,"no enough memory!\n");
		exit(-1);
	}
	
	page.set_page_size(page_size_);
	page.tobinary(bits);

	fseek(fp_,0,SEEK_SET);
	fwrite(&page_num_,sizeof(long),1,fp_);
	fwrite(&page_size_,sizeof(long),1,fp_);

	fseek(fp_,pageno*page_size_+2*sizeof(long),SEEK_SET);
	fwrite(bits,sizeof(char),page_size_,fp_);
	
	delete [] bits;
}

}//end of namespace 

#endif
