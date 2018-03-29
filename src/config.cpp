#pragma  once
#include "config.h"
#include <fstream>


bool IsSpace(char c)//判断是不是空格
{
    if (' ' == c || '\t' == c)
        return true;
    return false;
}

bool IsCommentChar(char c)//判断是不是注释符
{
    switch(c) {
        case COMMENT_CHAR:
            return true;
        default:
            return false;
    }
}

void Trim(string & str)//去除字符串的首尾空格
{
    if (str.empty()) {
        return;
    }
    int i, start_pos, end_pos;
    for (i = 0; i<str.size(); ++i) {
        if (!IsSpace(str[i])) {
            break;
        }
    }
    if (i == str.size()) { // 全部是空白字符串
        str = "";
        return;
    }

    start_pos = i;

    for (i = str.size() - 1; i >= 0; --i) {
        if (!IsSpace(str[i])) {
            break;
        }
    }
    end_pos = i;

    str = str.substr(start_pos, end_pos - start_pos + 1);
}
void  ReadProperties::initConfig(){
    char * cfgfilepath ="/home/hzl/project/github/echoServer/sipProject/src/config.properties";
    user=getValue(cfgfilepath,"user");
    password=getValue(cfgfilepath,"password");
    simulationDevice=getValue(cfgfilepath,"simulationDevice");
    serverIp=getValue(cfgfilepath,"serverIp");
    serverId=getValue(cfgfilepath,"serverId");
    myIp=getValue(cfgfilepath,"myIp");
    keepLiveSecond=getValue(cfgfilepath,"keepLiveSecond");

}

//参数name，配置项的名字
//返回值，对应配置项name的value值
string ReadProperties::getValue(const char * cfgfilepath,const string & name)
{
    fstream cfgFile;
    cfgFile.open(cfgfilepath);//打开文件
    if( ! cfgFile.is_open())
    {
        cout<<"can not open cfg file!"<<endl;
        return "";
    }
    char tmp[1000];
    while(!cfgFile.eof())//循环读取每一行
    {
        cfgFile.getline(tmp,1000);//每行读取前1000个字符，1000个应该足够了
        string line(tmp);
        size_t pos = line.find('=');//找到每行的“=”号位置，之前是key之后是value
        if(pos==string::npos) return false;
        string tmpKey = line.substr(0,pos);//取=号之前
        if(name==tmpKey)
        {
            return line.substr(pos+1);
        }
    }
    return "";
}

