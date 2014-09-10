#ifndef _RECORD_CPP
#define _RECORD_CPP

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "record.h"
using namespace std;

namespace flydb{

Record::Record()
    :bits_(NULL),len_(0)
{
}

Record::~Record()
{
    if(bits_ != NULL){
        delete [] bits_;
        bits_ = NULL;
    }
}
void Record::setbits(char *bits,int length)
{
    if(this->bits_){
        delete [] this->bits_;
        bits_ = NULL;
    }

    this->bits_ = bits;
    len_ = length;
}

void Record::copybits(const char *bits,int length)
{
    if(this->bits_){
        delete [] this->bits_;
        bits_ = NULL;
    }

    this->bits_ = new (std::nothrow) char[length];

    if(this->bits_ == NULL){
        fprintf(stderr,"no enough memory!\n");
        exit(-1);
    }

    memcpy(this->bits_,bits,length);

    len_ = length;
}
void Record::clone(const Record &other)
{
    copybits(other.bits_,other.len_);
}

Record::Record(const Record &other)
    :bits_(NULL),len_(0)
{
    *this = other;
}

Record &Record::operator = (const Record & other)
{
    copybits(other.bits_,other.len_);
    return *this;
}

void Record::project(int attrs_keep[],int num_keep)
{
    int tot_size = (num_keep + 1) * sizeof(int);

    int *index =(int *)bits_;

    int offset= 0 ,next_offset = 0;
    //计算投影后的Record的大小
    for(int i = 0; i < num_keep; ++i){
        next_offset = index[attrs_keep[i]+1];
        offset = index[attrs_keep[i]];

        tot_size += next_offset - offset;
    }

    //申请新的内存
    char * newbits = new (std::nothrow) char[tot_size];
    if(newbits == NULL){
        fprintf(stderr,"no enough memory!\n");
        exit(-1);
    }
    memset(newbits,0,tot_size);
    //将投影后的内容写到新的内存
    int *newindex = (int *)newbits;
    int length = 0;
    newindex[0] = (num_keep+1)*sizeof(int);
    for(int i = 0; i < num_keep; ++i){
        next_offset = index[attrs_keep[i]+1];
        offset = index[attrs_keep[i]];
        
        length = next_offset - offset;
        newindex[i+1] =  newindex[i] + length;

        memcpy((char *) &newbits[newindex[i]],(char *) &bits_[offset],length);
    }
    for(int i = 0 ; i <= num_keep; ++i){
        cout<< newindex[i]<<" ";
    }
    cout<<endl;

    if(this->bits_){
        delete [] bits_;
        bits_ = NULL;
    }
    bits_ = newbits;
    len_ = tot_size;
}
void Record::marge(const Record &left,const Record &right,int lattrs_keep[],int lnum_keep,int rattrs_keep[],int rnum_keep)
{
    if(lnum_keep == 0){
        clone(right);
        return;
    }else if(rnum_keep == 0){
        clone(left);
        return ;
    }

    int offset= 0 ,next_offset = 0;

    int tot_size = (lnum_keep + rnum_keep + 1) * sizeof(int);

    int *lindex =(int *)left.bits_;

    //计算合并后的Record的大小
    for(int i = 0; i < lnum_keep; ++i){
        next_offset = lindex[lattrs_keep[i]+1];
        offset = lindex[lattrs_keep[i]];

        tot_size += next_offset - offset;
    }

    int *rindex =(int *)right.bits_;

    for(int i = 0; i < rnum_keep; ++i){
        next_offset = rindex[rattrs_keep[i]+1];
        offset = rindex[rattrs_keep[i]];

        tot_size += next_offset - offset;
    }
    //申请新的内存
    char * newbits = new (std::nothrow) char[tot_size];
    if(newbits == NULL){
        fprintf(stderr,"no enough memory!\n");
        exit(-1);
    }
    cout<<"tot_size:"<<tot_size<<endl;
    memset(newbits,0,tot_size);
    //将合并后的内容写到新的内存
    int *newindex = (int *)newbits;
    int length = 0;
    newindex[0] = (lnum_keep + rnum_keep + 1)*sizeof(int);

    for(int i = 0; i < lnum_keep; ++i){
        next_offset = lindex[lattrs_keep[i]+1];
        offset = lindex[lattrs_keep[i]];
        
        length = next_offset - offset;
        newindex[i+1] =  newindex[i] + length;

        memcpy((char *) &newbits[newindex[i]],(char *) &left.bits_[offset],length);
    }

    cout<<"new index:";
    for(int i = 0 ; i <= lnum_keep+rnum_keep; ++i){
        cout<< newindex[i]<<" ";
    }
    cout<<endl;

    for(int i = 0; i < rnum_keep; ++i){
        next_offset = rindex[rattrs_keep[i]+1];
        offset = rindex[rattrs_keep[i]];
        
        length = next_offset - offset;
        newindex[i+lnum_keep+ 1] =  newindex[i+lnum_keep] + length;

        memcpy((char *) &newbits[newindex[i+lnum_keep]],(char *) &right.bits_[offset],length);
    }
    cout<<"new index:";
    for(int i = 0 ; i <= lnum_keep+rnum_keep; ++i){
        cout<< newindex[i]<<" ";
    }
    cout<<endl;

    if(this->bits_){
        delete [] bits_;
        bits_ = NULL;
    }

    bits_ = newbits;
    len_ = tot_size;
    
}

int Record::suck(Schema *schema,FILE *textfile)
{
    if(schema == NULL){
        return -1;
    }
	int ret = 0;
    char *space = new (std::nothrow) char[PAGE_SIZE];
    if(space == NULL){
        fprintf(stderr,"no enough memory!\n");
        exit(-1);
    }
    char *recspace = new (std::nothrow) char[PAGE_SIZE];
    if(recspace == NULL){
        fprintf(stderr,"no enough memory!\n");
        exit(-1);
    }

    memset(space,0,PAGE_SIZE);
    memset(recspace,0,PAGE_SIZE);

    int length = 0;
    char nextchar=0;

    int *index = (int *)recspace;
    
    int i = 0;
    index[0] = (schema->get_attr_num() + 1) * sizeof(int);

    for(Schema::iterator it = schema->begin(); it != schema->end(); ++it){
        length = 0;
        
        while(1){
            nextchar = getc(textfile);
            if(nextchar == '|'){
                break;
            }else if(nextchar == EOF){
				ret = -2;
				break;
				
            }else if(nextchar == '\r'|| nextchar == '\n'){
                continue;
            }else{
                space[length++] = nextchar;
            }
        }
       /* if(space[length] != 0){
            space[length++] = 0;
        }
        */
        switch(it->type){
            case T_INT: 
                if(length != 0){
                    index[i+1] = index[i] + sizeof(int);
                }else{
                    index[i+1] = index[i] ;
                }
                *(int *)(&recspace[index[i]]) = atoi(space);
                break;
            case T_DOUBLE: 
                if(length != 0){
                    index[i+1] = index[i] + sizeof(double);
                }else{
                    index[i+1] = index[i] ;
                }
                *(double *)(&recspace[index[i]]) = atof(space);
                break;
            case T_STRING: 
                index[i+1] = index[i] + length;
                memcpy((char *)(&recspace[index[i]]),space,length);

                break;
            default:
                fprintf(stderr,"error:unknown type\n");
				ret = -1;
        }

        ++i;

		if(ret == -2 && i != schema->get_attr_num()){
			ret = -1;
			break;
		}
    }
    
    if(this->bits_){
        delete [] bits_;
        bits_ = NULL;
    }

    len_ = index[i];
    bits_ = new(std::nothrow) char[len_];

    if(bits_ == NULL){
        fprintf(stderr,"no enought memory!\n");
        exit(-1);
    }

    memcpy(bits_,recspace,len_);

    delete [] space;
    delete [] recspace;
	
	return ret;
}

void Record::print(Schema *schema)
{
    if(schema == NULL){
        return;
    }

    int offset = 0;
    int next_offset = 0;
    int i = 0;

    int *myInt = NULL;
    double *myDouble = NULL;
    
    for(Schema::iterator it = schema->begin(); it != schema->end(); ++it){
        cout<<it->name<<": ";
        
        offset = ((int *)bits_)[i++];
        next_offset = ((int *)bits_)[i];
        
        if(offset == next_offset){
            cout<<"(null)"<<endl;
        }else{

            switch(it->type){
                case T_INT: 
                    myInt = (int *)(&bits_[offset]);
                    cout << *myInt<<endl;
                    break;
                case T_DOUBLE: 
                    myDouble = (double *)(&bits_[offset]);
                    cout << *myDouble<<endl;
                    break;
                case T_STRING: 
                    //char *myString = (char *)(&bits_[offset]);
                    //不要直接用cout<<myString<<endl;
                    //因为该字符串可能不是以'\0'作为结尾
                    //
                    for(int i=offset; i != next_offset; ++i){
                        cout<<bits_[i];
                    }
                    cout<<endl;
                    break;
                default:
                    cout<<"unknow type"<<endl;
            }
        }
    }

}

}//end of namespace 
#endif
