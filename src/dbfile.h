#ifndef _DBFILE_H
#define _DBFILE_H

namespace flydb{

class DBFile
{
public:
    DBFile();
    ~DBFile();

    int open(const char *fname);
    void close();

    void load(Schema *schema,const char *loadpath);

    void movetofirst();
    void add(Record &addme);
    //seq scan
    int getnext(Record &fetchme);

    //todo:filter seq scan
    //int getnext(Record &fetchme,)
private:
    File *heapfile_;
    Page *readpage_;
    Page *writepage_;
    char *fname;
};

}//end of namespace
#endif
