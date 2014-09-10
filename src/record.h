#ifndef _RECORD_H
#define _RECORD_H

#include "schema.h"

namespace flydb
{

#define PAGE_SIZE (1024)

class Record
{
public:
    Record();
    ~Record();
    int suck(Schema *schema,FILE *textfile);
    void print(Schema *schema);
    void clone(const Record & other);
    void project(int attrs_keep[],int num_keep);
    void marge(const Record &left,const Record &right,int lattrs_keep[],int lnum_keep,int rattrs_keep[],int rnum_keep);
    
    //deeply copy
    Record & operator = (const Record & other);
    Record(const Record &other);
private:            

    char *getbits(){ return bits_;}
    void setbits(char *bits,int lenth);
    void copybits(const char *bits,int lenth);
    

    int length(){ return len_; }
private:
    char *bits_;
    int len_;
};

}

#endif
