#include "rpc_provider.h"
#include <string>
#include "mprpc_application.h"
//这里是框架提供给外部使用的，可以发布RPC方法的函数接口
void RpcProvider::NotifyService(google::protobuf::Service *service)
{

}
//启动RPC服务节点，开始提供RPC远程网络调用服务
void RpcProvider::Run()
{
    std::string ip = MprpcApplication::GetInstance()GetConfig().Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverip").c_str());
    muduo::net::InetAddress address(ip,port);

    //正式开始启动
}
