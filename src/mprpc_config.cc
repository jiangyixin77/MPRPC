//和test.conf相关
#include "mprpc_config.h"
#include <string>
#include <iostream>

//解析加载配置文件
void MprpcConfig::LoadConfigFile(const char *config_file)
{
    FILE *pf = fopen(config_file,"r");
    if(nullptr == pf) {std::cout << config_file << "不存在！" << std::endl; exit(EXIT_FAILURE);}

    //注释，正确的配置项，去掉多余空格
    while (!feof(pf))
    {
        char buf[512] = {0};
        fgets(buf,512,pf);
        std::string read_buf(buf);
        Trim(read_buf);
    
        //判断#注释或空行并跳过
        if(read_buf[0] == '#' || read_buf.empty()) continue;

        //解析配置项
        int idx = read_buf.find('=');
        if(idx == -1) continue;//配置项不合法

        std::string key;
        std::string value;

        key = read_buf.substr(0,idx);
        Trim(key);
        int end_idx = read_buf.find('\n',idx);

        value = read_buf.substr(idx+1,end_idx-idx-1);
        Trim(value);
        m_configMap.insert({key,value});
    }
    
}

//查询配置项信息
std::string MprpcConfig::Load(const std::string &key)
{
    auto it = m_configMap.find(key);
    if(it == m_configMap.end()) return "";
    return it->second;
}

//去除字符串前后空格
void MprpcConfig::Trim(std::string &src_buf)
{
    //去掉字符串前面多余空格
    int idx = src_buf.find_first_not_of(' ');
    if(idx != -1)  src_buf = src_buf.substr(idx,src_buf.size()-idx);

    //去掉字符串后面多余空格
    idx = src_buf.find_last_not_of(' ');
    if(idx != -1)  src_buf = src_buf.substr(0,idx+1);
}
