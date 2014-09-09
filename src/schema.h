
#ifndef _SCHEMA_H
#define _SCHEMA_H

#include <map>
#include <vector>
#include <string>
#include <string.h>
#include "stringutil.h"

using namespace std;
using namespace stringutil;
namespace flydb
{
enum AttrType{ T_ERR=-1,T_INT,T_DOUBLE,T_STRING };

struct Attribute
{
	string name;
	AttrType type;

    static AttrType to_type(const string &strtype)
    {
        string lowtype=str_to_lower(strtype);
        if(lowtype == "int"){
            return T_INT;
        }else if(lowtype == "double"){
            return T_DOUBLE;
        }else if(lowtype == "string"){
            return T_STRING;
        }

        return T_ERR;
    }
    static string to_string(const AttrType type){
        string s;
        switch(type){
            case T_INT:s = "int";break;
            case T_DOUBLE:s = "double";break;
            case T_STRING:s = "string";break;
            default:break;
        }

        return s;
    }
};
class Schema
{
public:    
	typedef vector<Attribute>::iterator iterator;
	iterator begin() {return attrs_.begin();}
	iterator end() {return attrs_.end();}
public:    
    bool hasattr(const string &attrname);
	int  getattr(const string &attrname,Attribute &attr);
	int  gettype(const string &attrname,AttrType &type);
    int  get_attr_num() { return attrs_.size(); }
private:
	string name_;
	vector<Attribute> attrs_;
};

class SchemaMgr
{
public:	
	SchemaMgr();
	~SchemaMgr(){release();}

public:
	typedef map<string,Schema *>::iterator iterator;

	iterator begin() {return schemas_.begin();}
	iterator end() {return schemas_.end();}
public:
	int open(const string &fname);
	
	int getattr(const string &schema,const string &attrname,Attribute &attr);
	int gettype(const string &schema,const string &attrname,AttrType &type);
	
	bool hasschema(const string &schema) ;
	bool hasattr(const string &schema,const string &attr) ;
	
	Schema *getschema(const string &schema="");
private:
    //disallow copy
    SchemaMgr(const SchemaMgr &other);
    SchemaMgr &operator =(const SchemaMgr &other);

	void release();
	bool iscomment(const string &str);
	bool parse(const string &content,string &attr,string &type,char c= '=');
	//for dubug
	void print();
private:
	map<string,Schema *> schemas_;
	string fname_;
};

}

#endif
