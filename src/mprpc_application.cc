#include "mprpc_application.h"
#include <iostream>
#include <unistd.h>
#include <string>

MprpcConfig MprpcApplication::m_config;

void showArgsHelp()
{
    std::cout << "format:command -i <configfile>" << std::endl;
}
void MprpcApplication::Init(int argc,char **argv)
{
    if(argc < 2)
    {
        showArgsHelp();
        exit(EXIT_FAILURE);
    }
    int c = 0;
    std::string config_file;
    while((c = getopt(argc,argv,"i:")) != -1)
    {
        switch(c)
        {
            case 'i': config_file = optarg; break;
            case '?': showArgsHelp(); exit(EXIT_FAILURE);
            case ':': showArgsHelp(); exit(EXIT_FAILURE);
            default : break;
        }
    }
    //下面开始加载配置文件  rpcserver_ip rpcserver_port zookeeper_ip zookeeper_port
    m_config.LoadConfigFile(config_file.c_str());

    //输出相应信息
    std::cout << "rpc server ip = " << m_config.Load("rpcserverip");
    std::cout << "rpc server port = " << m_config.Load("rpcserverport");
    std::cout << "zookeeper ip = " << m_config.Load("zookeeperip");
    std::cout << "zookeeper port = " << m_config.Load("zookeeperport");
}
MprpcApplication& MprpcApplication::GetInstance()
{
    static MprpcApplication app;
    return app;
}
