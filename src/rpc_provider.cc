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
    uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    muduo::net::InetAddress address(ip,port);

    //正式开始启动
    //创建TcpServer对象
    muduo::net::TcpServer server(&m_eventLoop,address,"RpcProvider");
    //绑定连接回调方法和消息读写回调方法   【muduo好处=>分离网络代码和业务代码】
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection,this,std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    //设置muduo库线程数量
    server.setThreadNum(4);
    
    //测试
    //std::cout << "RpcProvider start service at" << ip << " " << port << std::endl;
    
    //启动网络服务
    server.start();
    m_eventLoop.loop(); 
}

//新的socket连接回调
void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr &conn)
{
    
}

//已建立连接用户的读写事件回调
void OnMessage(const muduo::net::TcpConnectionPtr&,muduo::net::Buffer*,muduo::Timestamp)
{
    
}
