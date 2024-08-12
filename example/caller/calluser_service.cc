#include <iostream>
#include "mprpc_application.h"
#include "user.pb.h"
#include "mprpc_channel.h"

int main(int argc,char **argv)
{
  //程序启动后，要想通过MPRPC框架来使用RPC服务调用，就必须先调用框架初始化函数（初始化一次即可）
  MprpcApplication::Init(argc,argv);

  //调用远程发布的RPC方法的Login方法
  fixbug::UserServiceRpc_Stub stub(new MprpcChannel());

  //RPC方法的Login方法的请求参数，姓名和密码
  fixbug::LoginRequest request;
  request.set_name("seven");
  request.set_pwd("12345");

  //RPC方法的Login方法的响应
  fixbug::LoginResponse response;

  //发起RPC方法调用，同步的RPC调用过程   MprpcChannel::callmethod
  //RpcChannel->RpcChannel::callmethod，集中来做所有所有RPC方法调用的参数序列化以及网络发送
  stub.Login(nullptr,&request,&response,nullptr);

  //RPC调用完成后，读取调用结果
  if(response.result().errcode() == 0) 
  {
    std::cout << "RPC登录响应成功：" << response.success() << std::endl;
  }
  else 
  {
    std::cout << "RPC登录响应失败：" << response.result().errmsg() << std::endl;
  }

  //调用远程发布的RPC方法Register
  fixbug::RegisterResponse rsp;

  //RPC方法的Register方法的请求参数，id，姓名和密码
  fixbug::RegisterRequest req;
  req.set_id(1);
  req.set_name("seven");
  req.set_pwd("12345");

  //以同步方式发起RPC调用请求并等待结果返回
  stub.Register(nullptr,&req,&rsp,nullptr);

  //RPC调用完成后，读取调用结果
  if(rsp.result().errcode() == 0) 
  {
    std::cout << "RPC注册响应成功：" << response.success() << std::endl;
  }
  else 
  {
    std::cout << "RPC注册响应失败：" << response.result().errmsg() << std::endl;
  }
  return 0;
}
