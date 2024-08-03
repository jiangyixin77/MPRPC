#include <iostream>
#include <string>
#include "../user.pb.h" 
using namespace fixbug;
//UserService原来是本地服务，提供两个进程内的本地方法，Login和GetFriendLists
class UserService : public fixbug::UserServiceRpc  //使用在RPC服务发布端
{
    public:
    bool Login(std::string name,std::string pwd)
    {
        std::cout << "name:" << name << "pwd:" << pwd << std::endl;
    }
    //重写基类UserServiceRpc虚函数          【下面方法都是框架直接调用的】
    //1.caller -> Login(LoginRequest) -> muduo -> callee
    //2.callee -> Login(LoginRequest) -> 交由下面重写的这个Login方法
    void Login(::google::protobuf::RpcController* controller,
                       const ::fixbug::LoginRequest* request,
                       ::fixbug::LoginResponse* response,
                       ::google::protobuf::Closure* done)
    {
        //框架给业务上报了请求参数LoginRequest，应用获取相应数据做本地业务
        std::string name = request->name();
        std::string pwd = request->pwd();

        //做本地业务
        bool login_result = Login(name,pwd);

        //把响应写入    含错误码、错误消息、返回值
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(login_result);

        //执行回调操作  执行响应对象数据的序列化和网络发送（都是由框架来完成的）
        done->Run();
    }
};

int main()
{

    return 0;
}
