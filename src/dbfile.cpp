#ifndef _DBFILE_CPP
#define _DBFILE_CPP

#include "dbfile.h"

namespace flydb
{

DBFile::DBFile()
    :fname_(NULL)
{
    heapfile_ = new File;
    readpage_ = new readpage_;
    writepage_ = new writepage_;
}

DBFile::~DBFile()
{
    delete heapfile_;
    delete readpage_;
    delete writepage_;
    delete fname;
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

    return heapfile_->open(fname_);
}

}//end of namespace

#endif
