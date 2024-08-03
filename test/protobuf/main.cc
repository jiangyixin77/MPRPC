//本文件将实现序列化与反序列化
#include "test.pb.h"
#include <iostream>
#include <string>
using namespace fixbug;

int main()
{
    GetFriendListsResponse rsp;
    ResultCode *rc = rsp.mutable_result();
    rc->set_errcode(0);

    User *u1 = rsp.add_friend_list();
    u1->set_name("Seven");
    //...

    std::cout << rsp.friend_lists_size() << std::endl;

    return 0;
}
/*
int main1()
{
    //封装了login请求对象的数据
    LoginRequest req;
    req.set_name("seven");
    req.set_pwd("123456");

    //对象数据序列化->char*
    std::string send_str;
    if(req.SerializeToString(&send_str))
    {
        std::cout << send_str.c_str() << std::endl;
    }

    //从send_str反序列化一个login请求对象
    LoginRequest req2;
    if(req2.ParseFromString(send_str))
    {
        std::cout << req2.name() << std::endl;
        std::cout << req2.pwd() << std::endl;
    }
    return 0;
}
*/
