#pragma  once
#include <string>
#include <iostream>
#define COMMENT_CHAR '#'//注释符
using namespace std;



class ReadProperties
{
private :
	ifstream *infile;
public:
	string user;
	string password;
	string simulationDevice;
	string serverIp;
	string serverId;
	string myIp;

	//参数name，配置项的名字
	//返回值，对应配置项name的value值
	string getValue(const char * cfgfilepath,const string & name);
	void  initConfig();
};