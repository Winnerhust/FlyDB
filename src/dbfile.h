#ifndef _DBFILE_H
#define _DBFILE_H

#include "record.h"
#include "schema.h"
#include "file.h"
#include "page.h"

namespace flydb{

class DBFile
{
public:
    DBFile();
    ~DBFile();

    int open(const char *fname);
    void close();

	void load(const char *loadpath,Schema &schema);

    void movetofirst();
    void add(Record &addme);
    //seq scan
    int getnext(Record &fetchme);

    //todo:filter seq scan
    //int getnext(Record &fetchme,)
private:
	void addwritepage();
	bool writebufempty() const;
private:	
    File *heapfile_;
    Page *readpage_;
    Page *writepage_;
    char *fname_;
	long pageno_;
};

}//end of namespace
#endif
