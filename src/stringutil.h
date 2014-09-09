#ifndef _STRINGUTIL_H
#define _STRINGUTIL_H

#include<string>
using namespace std;

namespace stringutil
{
	/*去掉str后面的c字符*/
	void trimleft(string &str,char c=' ');
	/*去掉str前面的c字符*/
	void trimright(string &str,char c=' ');
	/*去掉str前面和后面的空格符,Tab符等空白符*/
	void trim(string &str);
    string str_to_lower(const string &str);
    string str_to_upper(const string &str);

    int getline(string &str,FILE *fp);
}

#endif
