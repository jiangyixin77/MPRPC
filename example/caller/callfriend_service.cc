#include <iostream>
#include "mprpc_application.h"
#include "friend.pb.h"
#include "mprpc_channel.h"

int main(int argc,char **argv)
{
  //程序启动后，若通过MPRPC框架来调用RPC服务，必须先对框架进行初始化
  MprpcApplication::Init(argc,argv);

  //调用远程发布的RPC方法的Login方法
  fixbug::FriendServiceRpc_Stub stub(new MprpcChannel());

  //RPC方法请求
  fixbug::GetFriendsListRequest request;
  request.set_userid(3000);

  //RPC方法响应
  fixbug::GetFriendsListResponse response;

  //发起RPC方法的调用
  MprpcController controller;
  stub.GetFriendsList(nullptr,&request,&response,nullptr);

  //RPC调用完成，读取结果
  if(!controller.Failed())
  {
    std::cout << "RPC获得好友列表 响应成功" << std::endl;
    int size = response.friends_size();
    for(int i=0;i<size;i++)
      {
        std::cout << "index:" << (i+1) << "name:" <<response.friends(i) <<std::endl;
      }
  }
  else
  {
    std::cout << controller.ErrorText() << std::endl;
  }
}
