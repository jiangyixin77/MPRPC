#include "mprpc_channel.h"
#include "mprpc_application.h"
#include "mprpc_controller.h"
#include <string>
#include "rpcheader.pb.h"

//header_size + service_name method_name args_size + args

//所有通过stub代理对象调用的RPC方法，都集中到这里， 并统一做RPC方法调用的数据序列化和网络发
void CallMethod(const google::protobuf::MethodDescriptor* method,
                  google::protobuf::RpcController* controller,
                  const google::protobuf::Message* request,
                  google::protobuf::Message* response,
                  google::protobuf::Closure* done)
{
  //定义sd
  const google::protobuf::ServiceDescriptor* sd = method->service();
  std::string service_name = sd->name();    
  std::string method_name = method->name();

  //获取参数的序列化字符串长度args_size
  int args_size = 0;
  std::string args_str;
  if(request->SerializeToString(&args_str)) 
    args_size = args_str.size();
  else 
  {
    //std::cout << "序列化请求失败！" <<std::endl;
    controller->SetFailed("序列化请求失败！");
    return;
  }

  //定义RPC的请求header
  mprpc::RpcHeader rpcHeader;
  rpcHeader.set_service_name(service_name);
  rpcHeader.set_method_name(method_name);
  rpcHeader.set_args_size(args_size);

  //获取RPC header的字符串长度header_size
  uint32_t header_size = 0;
  std::string rpc_header_str;
  if(request->SerializeToString(&rpc_header_str)) 
  {
    header_size = rpc_header_str.size();
  }
  else 
  {
    //std::cout << "序列化RPC header失败！" <<std::endl; 
    controller->SetFailed("序列化RPC header失败！");
    return;
  }

  //组织待发送的RPC请求字符串send_rpc_str
  std::string send_rpc_str;
  send_rpc_str.insert(0,std::string((char*)&header_size,4));
  send_rpc_str += rpc_header_str;
  send_rpc_str += args_str;

  //下面开始打印调试信息
  std::cout << "header_size:" << header_size << std::endl;
  std::cout << "rpc_header_str:" << rpc_header_str << std::endl;
  std::cout << "service_name:" << service_name << std::endl;
  std::cout << "method_name:" << method_name << std::endl;
  std::cout << "args_str:" << args_str << std::endl;

  //使用TCP编程来完成RPC方法的远程调用  AF_INET什么意思？
  int clientfd = socket(AF_INET,SOCK_STREAM,0);
  if(clientfd == -1) 
  {
    //std::cout << "套接字创建失败！错误码：" << errno << std::endl;  
    char errtxt[512] = {0};
    sprintf(errtxt,"套接字创建失败！错误码：%d",errno);
    controller->SetFailed(errtxt);
    return;
  }

  //读取配置文件RPCServer的IP地址和接口信息
  //为什么使用uint16_t数据类型？
  std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
  uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

  //下面开始连接RPC的服务节点
  if(connect(clientfd,(struct sockaddr*)&server_addr,sizeof(server_addr)) == -1)
  {
    //std::cout << "连接失败！错误码：" << errno << std::endl;
    char errtxt[512] = {0};
    sprintf(errtxt,"连接失败！错误码：%d",errno);
    controller->SetFailed(errtxt);
    return;
  }

  //发送RPC请求
  if(send(clientfd,send_rpc_str.c_str(),send_rpc_str.size(),0) == -1)
  {
    //std::cout << "发送失败！错误码：" << errno << std::endl;
    char errtxt[512] = {0};
    sprintf(errtxt,"发送失败！错误码：%d",errno);
    controller->SetFailed(errtxt);
    return;
  }

  //接收RPC请求的响应值
  char recv_buf[1024] = {0};
  int recv_size = recv(clientfd,recv_buf,1024,0);
  if(recv(clientfd,recv_buf,1024,0) == -1)
  {
    //std::cout << "接收失败！错误码：" << errno << std::endl;
    char errtxt[512] = {0};
    sprintf(errtxt,"接收失败！错误码：%d",errno);
    controller->SetFailed(errtxt);
    close(clientfd);
    return;
  }

  //反序列化RPC调用的响应数据
  //问题：recv_buf中，遇到\0后面的数据就存不下来了，导致反序列化失败
  std::string response_str(recv_buf,0,recv_size);
  if(!response->ParseFromArray(recv_buf,recv_size))
  {
    //std::cout << "错误！响应字符串：" << response_str << std::endl;
    char errtxt[512] = {0};
    sprintf(errtxt,"错误！响应字符串：%d",response_str);
    close(clientfd);
    return;
  }
  close(clientfd);
}
