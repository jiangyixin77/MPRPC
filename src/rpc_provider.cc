#include "rpc_provider.h"
#include "rpcheader.pb.h"
#include "mprpc_application.h"


//service_name -> service描述 【service* 记录服务对象】【method_name -> method方法对象】


//这里是框架提供给外部使用的，可以发布RPC方法的函数接口
void RpcProvider::NotifyService(google::protobuf::Service *service)
{
    ServiceInfo service_info;
    //获取了服务对象的描述信息
    const google::protobuf::ServiceDescriptor *pserviceDesc = service GetDescriptor();
    //获取服务名字
    std::string service_name = pserviceDesc -> name();
    //获取服务对象service方法数量
    int methodCnt = pserviceDesc -> method_count();

    //service_name打印测试：
    std::cout << "service_name:" << service_name << std::endl;
    
    for(int i=0; i<methodCnt; i++)
    {
        //获取了服务对象指定下标的服务方法的描述（抽象描述） UserService Login
        const google::protobuf::MethodDescriptor* pmethodDesc = pserviceDesc -> method(i);
        std::string method_name = pmethodDesc->name();
        service_info.m_methodMap.insert({method_name,pmethodDesc});
    }
    service_info.m_service = service;
    m_serviceMap.insert({service_name,service_info});
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
    if(!conn->connected()) conn->shutdown();  //和RPC连接断开
}

/*
在框架内部，RPCProvider和RPCConsumer协商好之间通信用的protobuf数据类型
service_name    method_name   args    定义proto的message类型对数据头序列化/反序列化
示例：16UserServiceLoginJiang Yixin123456
示例结构：header_size{4字节} + header_str + args_str
*/

//已建立连接用户的读写事件回调  若远程有一个RPC服务调用请求，那么OnMessage方法就会响应
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr &conn,  muduo::net::Buffer *buffer,  muduo::Timestamp)
{
    //网络上接收的远程RPC调用请求的字符流  login args
    std::string recv_buf = buffer->retrieveAllAsString();
    
    //从字符流读取前4字节内容   这里的int不能单纯转化为string，而是要转化为二进制并存入string
    uint32_t header_size = 0;
    recv_buf.copy((char*)&header_size,4,0);
    
    //根据header_size读取数据头原始字符流，反序列化，得到RPC请求详细信息
    std::string rpc_header_str = recv_buf.substr(4,header_size);
    mprpc::RpcHeader rpcHeader;
    if(rpcHeader.ParseFromString(rpc_header_str))
    {
        //数据头反序列化成功
    }
    else
    {
        //数据头反序列化失败
    }

    //获取RPC方法参数字符流数据

    //打印调试信息
}
