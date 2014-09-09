#ifndef _STRINGUTIL_CPP
#define _STRINGUTIL_CPP

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "stringutil.h"
namespace stringutil
{
    int INI_BUF_SIZE=2048;

	void trimleft(string &str,char c/*=' '*/)
	{
		//trim head
	
		int len = str.length();

		int i = 0;
		while(str[i] == c && str[i] != '\0'){
			i++;
		}
		if(i != 0){
			str = string(str,i,len-i);
		}
	}
	
	void trimright(string &str,char c/*=' '*/)
	{
		//trim tail
		int i = 0;
		int len = str.length();
			
		
		for(i = len - 1; i >= 0; --i ){
			if(str[i] != c){
				break;
			}
		}

		str = string(str,0,i+1);
	}

	void trim(string &str)
	{
		//trim head
	
		int len = str.length();

		int i = 0;
		while(isspace(str[i]) && str[i] != '\0'){
			i++;
		}
		if(i != 0){
			str = string(str,i,len-i);
		}

		//trim tail
		len = str.length();

		for(i = len - 1; i >= 0; --i ){
			if(!isspace(str[i])){
				break;
			}
		}
		str = string(str,0,i+1);
	}
    string str_to_lower(const string &str)
    {
        string t=str;
        for(unsigned int i = 0; i < str.length(); ++i){
            t[i]=tolower(str[i]);
        }
        return t;
    }
    string str_to_upper(const string &str)
    {
        string t=str;
        for(unsigned int i = 0; i < str.length(); ++i){
            t[i]=toupper(str[i]);
        }
        return t;
    }
    int getline(string &str,FILE *fp)
    {
	    int plen = 0;
	    int buf_size = INI_BUF_SIZE*sizeof(char);

	    char *buf =(char *) malloc(buf_size);
	    char *pbuf = NULL;
	    char * p = buf;
	
	    if(buf == NULL){
		    fprintf(stderr,"no enough memory!exit!\n");
		    exit(-1);
	    }
	
	    memset(buf,0,buf_size);
	    int total_size = buf_size;
	    while(fgets(p,buf_size,fp) != NULL){
		    plen = strlen(p);

		    if( plen > 0 && p[plen-1] != '\n' && !feof(fp)){
	
			    total_size = strlen(buf)+buf_size;
			    pbuf = (char *)realloc(buf,total_size);
			
			    if(pbuf == NULL){
				    free(buf);
				    fprintf(stderr,"no enough memory!exit!\n");
				    exit(-1);
			    }

			    buf = pbuf;
			
			    p = buf + strlen(buf);
			
			    continue;
		    }else{
			    break;
		    }
	    }

	    str = buf;
	
	    free(buf);
	    buf = NULL;
	    return str.length();
    }
}//end of namespace stringutil
#endif
