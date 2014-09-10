
#ifndef _PAGE_CPP
#define _PAGE_CPP

#include <stdio.h>
#include <stdlib.h>
#include <list>
#include "page.h"

namespace flydb{

Page::Page()
    :freespace_(0),num_record_(0),page_size_(PAGE_SIZE)
{
}

Page::~Page()
{
    clear();
}

inline PageHead * Page::getindex(int num_record,char *bits)
{
    return (PageHead*) &(bits[page_size_-sizeof(int)*(3+num_record)]); 
}

inline void Page::setindex(int num_record,char *bits,unsigned int value,unsigned int isdelete)
{
    ((PageHead *)&(bits[page_size_-sizeof(int)*(3+num_record)]))->pos = value;
    ((PageHead *)&(bits[page_size_-sizeof(int)*(3+num_record)]))->isdelete = isdelete;
}

void Page::tobinary(char *bits)
{
    int curpos = 0;
    int i = 0;
    for(list<Record>::iterator it = records_.begin(); it != records_.end(); ++it,++i){
        memcpy((char * ) &bits[curpos],it->getbits(),it->length());
        
        setindex(i,bits,curpos,0);

        curpos += it->length();
        
        if( curpos + (i+2)*sizeof(int) > page_size_){
            fprintf(stderr,"page is overflow!\n");
            break;
        }
    }

    setindex(i,bits,curpos,0);

    *(int *) &bits[page_size_-sizeof(int)] = curpos;
    *(int *) &bits[page_size_-2*sizeof(int)] =  i;
    
}

void Page::frombinary(char *bits)
{
    freespace_  = *(int *) &bits[page_size_ - sizeof(int)];
    num_record_ = *(int *) &bits[page_size_ - 2*sizeof(int)];

    Record record ;
    
    
    PageHead *ph = NULL;
    PageHead *next_ph = NULL;
    for(int i = 0; i < num_record_; ++i){
        ph = getindex(i,bits);
        next_ph = getindex(i+1,bits);

        if(ph->pos >= freespace_){
            break;
        }else if(ph->isdelete == 1){
            continue;
        }else{
            record.copybits((char *)&bits[ph->pos],next_ph->pos - ph->pos);

            records_.push_back(record);
        }

    }
    
    //去掉被删除的记录，重新调整记录数目
    num_record_ = records_.size();

}

int Page::append(Record &addme)
{
    if(freespace_ + addme.length() +(num_record_ + 2 + 1)*sizeof(int) > page_size_){
        fprintf(stderr,"page cannot add this record\n");
        return -1;
    }
    

    freespace_ += addme.length();

    num_record_++;

    records_.push_back(addme);

    return 0;
}

int Page::fetchfirst(Record &rec)
{
    if(records_.size() <= 0){
        return -1;
    }

    rec.clone(*records_.begin());

    records_.pop_front();

    return 0;
}
void Page::clear()
{

    records_.clear();

    freespace_ = 0;
    num_record_ = 0;
}

} //endof namespace
#endif
