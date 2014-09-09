#include <iostream>
#include <gtest/gtest.h>
#include "page.h"

using namespace std;
using namespace flydb;

TEST(Page,append)
{
    //写入数据文件
    char content[]="2005||94.5|'2014-09-07 22:29:10'|\n2006|cxy||'2014-9-8 13:38:35'|\n|yyy|93.5|'2014-9-8 13:39:37'|\n2008|ttt|88|'2014-9-8 13:40:05'|";

    char suckfilepath[] = "test/page.txt";
    FILE *fp = fopen(suckfilepath,"w");

	fwrite(content,1,sizeof(content),fp);
	fclose(fp);
    
    //加载Schema
	char filepath[30] = "test/record_test.ini";
	SchemaMgr ini;
	ini.open(filepath);

	EXPECT_EQ(ini.hasschema("Scores"),true);

    Schema *schema = ini.getschema("Scores");
    
    //suck
    fp = fopen(suckfilepath,"r");
    
    Record record[4];
    
    for(int i = 0; i < 4; ++i){
     
        record[i].suck(schema,fp);
        record[i].print(schema);
    }

    fclose(fp);
    
    
    Page page;
    for(int i = 0; i < 4 ; ++i){
        page.append(record[i]);
    }

    EXPECT_EQ(page.get_num_record(),4);
    
   
}
