#pragma once
#include "google/protobuf/service.h"
#include <unordered_map>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <string>
#include <functional>
#include <google/protobuf/descriptor.h>

//框架提供的专门发布RPC服务的网络对象类
class RpcProvider
{
public:
    //这里是框架提供给外部使用的，可以发布RPC方法的函数接口
    void NotifyService(google::protobuf::Service *service);

    //启动RPC服务节点，开始提供RPC远程网络调用服务
    void Run();
private:
    //组合了EventLoop
    muduo::net::EventLoop m_eventLoop;

    //service服务类型信息
    struct ServiceInfo
    {
        google::protobuf::Service *m_service; //保存服务对象
        std::unordered_map<std::string , const google::protobuf::MethodDescriptor*> m_methodMap; //保存服务方法
    }

    //存储注册成功的服务对象及其方法的所有信息
    std::unordered_map<std::string , ServiceInfo> m_serviceMap;

    //新的socket连接回调
    void OnConnection(const muduo::net::TcpConnectionPtr&);

    //已建立连接用户的读写事件回调
    void OnMessage(const muduo::net::TcpConnectionPtr&,muduo::net::Buffer*,muduo::Timestamp);
};
