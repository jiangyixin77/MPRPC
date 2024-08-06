#pragma once
#include "google/protobuf/service.h"
#include <memory>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>

//框架提供的专门发布RPC服务的网络对象类
class RpcProvider
{
public:
    //这里是框架提供给外部使用的，可以发布RPC方法的函数接口
    void NotifyService(google::protobuf::Service *service);

    //启动RPC服务节点，开始提供RPC远程网络调用服务
    void Run();
private:
    muduo::net::EventLoop m_eventLoop;//组合了EventLoop
    void OnConnection(const muduo::net::TcpConnectionPtr&);//新的socket连接回调
    void OnMessage(const muduo::net::TcpConnectionPtr&,muduo::net::Buffer*,muduo::Timestamp);//已建立连接用户的读写事件回调
};
