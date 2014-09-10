#ifndef _DBFILE_CPP
#define _DBFILE_CPP

#include "dbfile.h"

namespace flydb
{

DBFile::DBFile()
    :fname_(NULL),pageno_(0)
{
    heapfile_ = new File;
    readpage_ = new Page;
    writepage_ = new Page;
}

DBFile::~DBFile()
{
    delete heapfile_;
    delete readpage_;
    delete writepage_;
    free(fname_);
}

int DBFile::open(const char *fname)
{
    if(fname){
        fname_ = strdup(fname);
        if(fname_ == NULL){
            fprintf(stderr,"no enough memory!\n");
            exit(-1);
        }
    }

    int ret = heapfile_->open(fname_);
	if(ret != -1){
		heapfile_->getpage(*readpage_,0);
	}

	return ret;
}

void DBFile::add(Record &addme)
{
	long ret = writepage_->append(addme);

	if(ret == -1){
		heapfile_->addpage(*writepage_,heapfile_->get_page_num());				

		writepage_->clear();

		writepage_->append(addme);
	}
}

int DBFile::getnext(Record &fetchme)
{
	if(readpage_->fetchfirst(fetchme) == -1){
		if(pageno_ < heapfile_->get_page_num() - 1){
			++pageno_;
		}else {
			if(!writebufempty()){
				addwritepage();
			}else{
				return -1;
			}
		}

		readpage_->clear();

		if(heapfile_->getpage(*readpage_,pageno_) == -1){
			return -1;
		}
			
		if(readpage_->fetchfirst(fetchme) == -1){
			return -1;
		}
	}

	return 0;
}

void DBFile::addwritepage()
{
	if(!writebufempty()){
		heapfile_->addpage(*writepage_,heapfile_->get_page_num());
		writepage_->clear();
	}
}
void DBFile::load(const char *loadpath,Schema &schema)
{
	Record temp;
	FILE *tablefile = fopen(loadpath,"r");

	if(tablefile == NULL){
		fprintf(stderr,"load path [%s] does not exits!\n",loadpath);
		return;
	}

	while(temp.suck(&schema,tablefile) != -1){
		add(temp);
	}

	fclose(tablefile);

}

bool DBFile::writebufempty() const
{
	return (writepage_->get_num_record() == 0);
}

void DBFile::movetofirst()
{
	readpage_->clear();

	heapfile_->getpage(*readpage_,0);

	pageno_ = 0;
}

void DBFile::close()
{
	if(!writebufempty()){
		addwritepage();
	}

	heapfile_->close();
}

}//end of namespace

#endif
