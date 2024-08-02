//muduo网络库给用户提供了两个主要类
//TcpServer  TcpClient

//epoll+线程池      把网络I/O的代码和业务代码【用户连接断开&用户可读写事件】区分开

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <string>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;

//基于muduo库，开发Server程序
class CServer
{
public:
    CServer(EventLoop *loop,    //事件循环
           const InetAddress &listenAddr,  //IP+Port
           const string &nameArg)    //服务器的名字
        : _server(loop,listenAddr,nameArg),_loop(loop)
    {
        //给Server注册用户连接的创建和断开回调函数
        _server.setConnectionCallback(std::bind(&CServer::onConnection,this,_1));
        //给Server注册用户读写事件回调函数
        _server.setMessageCallback(std::bind(&CServer::onMessage,this,_1,_2,_3));//末尾都是占位符
        //5.设置Server端线程数量  1个I/O线程+3个worker线程
        _server.setThreadNum(4);
    }
    //3.明确TCPServer构造函数需要什么参数，输出CServer构造函数
    //4.在CServer构造函数中，注册两大回调函数

    //6.开启事件循环
    void start()
    {
        _server.start();
    }
private:
    //专门处理用户连接的创建和断开  epoll listenfd accept
    void onConnection(const TcpConnectionPtr& conn)
    {
        if(conn->connected())
        {
            cout << conn->peerAddress().toIpPort() << "->" << conn->localAddress().toIpPort() << "state:online"<<endl;
        }
        else
        {
            cout << conn->peerAddress().toIpPort() << "->" << conn->localAddress().toIpPort() << "state:ofline"<<endl;
            conn->shutdown();
        }
    }
    //专门处理用户读写事件
    void onMessage(const TcpConnectionPtr &conn,//连接
                  Buffer *buffer,//缓冲区
                  Timestamp time)//接受数据的时间
    {
        string buf = buffer->retrieveAllAsString();
        cout<<"recv data:"<<buf<<"time:"<<time.toString()<<endl;
        conn->send(buf);
    }
    TcpServer _server; //1.组合TCP Server对象
    EventLoop *_loop; //2.创建EventLoop事件循环对象的指针
                                  
};
int main()
{
    EventLoop loop;//epoll
    InetAddress addr("127.0.0.1",6000);
    CServer server(&loop,addr,"CServer");

    server.start();//listenfd epoll_ctl=>epoll
    loop.loop();//epoll_wait以阻塞方式等待[新用户连接]或[已连接用户的读写事件等]
    return 0;
}
