#include <iostream>
#include <string>
#include <vector>
#include "friend.pb.h"
#include "mproc_application.h"
#include "rpc_provider.h"

class FriendService:public fixbug::FriendServiceRpc
{
public:
  std::vector<std::string> GetFriendsList(uint32_t userid)
  {
    std::cout << "启动获取好友列表服务" << std::endl;
    std::vector<std::string> vec;
    vec.push_back("Amy");
    vec.push_back("Bob");
    vec.push_back("Cathy");
    return vec;
  }

  //对父类方法重写
  void GetFriendsList(::google::protobuf::RpcController* controller,
                      const ::fixbug::GetFriendsListRequest* request,
                      ::fixbug::GetFriendsListResponse* response,
                      ::google::protobuf::Closure* done)
  {
    uint32_t userid = request->userid();
    std::vector<std::string> friendsList = GetFriendsList(userid);
    response->mutable_result()->set_errcode(0);
    response->mutable_result()->set_errmsg("");
    for(std::string &name : friendsList)
    {
      std::string *p = response->add_friends();
      *p = name;
    }
    done->Run();
  }
}
int main(int argc,char **argv)
{
  //框架初始化
  MprpcApplication::Init(argc,argv);

  //把UserService对象发布到RPC节点上
  RpcProvider provider;
  provider.NotifyService(new FriendService());

  //启动RPC服务发布节点
  //启动后，进程阻塞并等待远程RPC调用请求
  provider.Run();

  return 0;
}
