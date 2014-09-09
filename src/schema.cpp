
#ifndef _SCHEMA_CPP
#define _SCHEMA_CPP

#include "stringutil.h"
#include "schema.h"
#include <stdio.h>
namespace flydb{
using namespace stringutil;

bool Schema::hasattr(const string &attrname)
{
    for(iterator it = attrs_.begin(); it != attrs_.end(); ++it){
        if(it->name == attrname){
            return true;
        }
    }
    return false;
}

int  Schema::getattr(const string &attrname,Attribute &attr)
{
    for(iterator it = attrs_.begin(); it != attrs_.end(); ++it){
        if(it->name == attrname){
            attr = *it;
            return 0;
        }
    }

    return -1;

}

int  Schema::gettype(const string &attrname,AttrType &type)
{
    for(iterator it = attrs_.begin(); it != attrs_.end(); ++it){
        if(it->name == attrname){
            type = it->type;
            return 0;
        }
    }

    return -1;

}

/**************************************************/

SchemaMgr::SchemaMgr()
{
}
bool SchemaMgr::parse(const string &content,string &key,string &value,char c/*= '='*/)
{
	int i = 0;
    
	int len = content.length();

	while(i < len && content[i] != c){
		++i;
	}
	if(i >= 0 && i < len){
		key = string(content.c_str(),i);
		value = string(content.c_str()+i+1,len-i-1);
		return true;
	}

	return false;
}

int SchemaMgr::open(const string &filename)
{	
	release();

	Schema *schema = NULL;
	string line;
	fname_ = filename;
	FILE *fp = fopen(filename.c_str(),"r");

	
	if(fp == NULL ){
        fprintf(stderr,"failed to open file: %s!\n",filename.c_str());
		return -1;
	}

	schema = new Schema();
	schemas_[""] = schema;


	while(getline(line,fp) > 0){
		
		trimright(line,'\n');
		trimright(line,'\r');
		trim(line);
		
		if(line.length() <= 0){
			continue;
		}

		if(line[0] == '['){
			schema = NULL;
			int index = line.find_first_of(']');

			if(index == -1){
				fclose(fp);
				fprintf(stderr,"没有找到匹配的]\n");
				return -1;
			}
			int len = index-1;
			if(len <= 0){
				fprintf(stderr,"段为空\n");
				continue;
			}
			string s(line,1,len);

			if(getschema(s.c_str()) != NULL){
				fclose(fp);
				fprintf(stderr,"此段已存在:%s\n",s.c_str());
				return -1;
			}
			
			schema = new Schema();
			schemas_[s] = schema;

			schema->name_ = s;
		}else if(iscomment(line)){
            continue;
		}else{
			string key,value;
			if(parse(line,key,value)){
                if(key=="col_info"){
			        int index = value.find_first_of(',');

			        if(index == -1 ){
				        fclose(fp);
				        fprintf(stderr,"failed to parse col_info \n");
				        return -1;
			        }
                    string name(value,0,index);
			        string type(value,index+1,value.length()-index);
                    
                    trim(name);
                    trim(type);

				    Attribute item;

				    item.name = name;
				    item.type = Attribute::to_type(type);

				    schema->attrs_.push_back(item);
                }
			}else{
				fprintf(stderr,"解析参数失败[%s]\n",line.c_str());
			}
		}

	}
	
	fclose(fp);

    return 0;
}

Schema *SchemaMgr::getschema(const string &schema /*=""*/)
{
	iterator it = schemas_.find(schema);
	if(it != schemas_.end()){
		return it->second;
	}

	return NULL;
}

int SchemaMgr::getattr(const string &schema,const string &attrname,Attribute &attr)
{
    Schema *s = getschema(schema);

    if(s != NULL){
        return s->getattr(attrname,attr);
    }

    return -1;
}

int SchemaMgr::gettype(const string &schema,const string &attrname,AttrType &type)
{
    Schema *s = getschema(schema);

    if(s != NULL){
        return  s->gettype(attrname,type);
    }

    return -1;


}
	
bool SchemaMgr::hasschema(const string &schema) 
{
    return ( (getschema(schema) != NULL) ? true : false);
}
bool SchemaMgr::hasattr(const string &schema,const string &attrname) 
{
    Schema *s = getschema(schema);

    if(s != NULL){
        return s->hasattr(attrname);
    }

    return false;

}

void SchemaMgr::release()
{
	fname_ = "";

	for(iterator i = schemas_.begin(); i != schemas_.end(); ++i){
		delete i->second;
	}

	schemas_.clear();

}

bool SchemaMgr::iscomment(const string &str)
{
    if( (str.length() > 0) && ( (str[0] == '#') || (str[0] == ';'))){
        return true;
    }

	return false;
}
//for debug
void SchemaMgr::print()
{
    string type;
	printf("filename:[%s]\n",fname_.c_str());

	for(iterator it = schemas_.begin(); it != schemas_.end(); ++it){
		printf("schema:[%s]\n",it->first.c_str());
		for(Schema::iterator i = it->second->attrs_.begin(); i != it->second->attrs_.end(); ++i){
            type = Attribute::to_string(i->type);
			printf("    parm   :%s=%s\n",i->name.c_str(),type.c_str());
		}
	}
}
}
#endif
