#ifndef _PAGE_H
#define _PAGE_H

#include <list>
#include "record.h"

namespace flydb{


struct PageHead
{
    //此处限制了可以表示的数据内容最多为2^15字节，即32768（8K）个字节
    unsigned int pos:15;
    unsigned int isdelete:1;
};
class Page
{
public:
    Page();
    ~Page();
    void tobinary(char *bits);
    void frombinary(char *bits);
    int get_num_record(){ return num_record_; }

    int get_page_size(){ return page_size_; }
    void set_page_size(int page_size){ page_size_ = page_size; }

    int append(Record &addme);
    int fetchfirst(Record &rec);
    bool hasnext(){ return (records_.size() != 0); }
    void clear();
private:
    PageHead * getindex(int num_record,char *bits);
    void setindex(int num_record,char *bits,unsigned int pos,unsigned int isdelete);
private:    
    int freespace_;
    int num_record_;
    unsigned int page_size_;
    list<Record> records_;
};

} //endof namespace
#endif
