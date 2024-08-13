#include "zookeeperutil.h"	
#include "mprpcapplication.h"
 #include <semaphore.h>	
 #include <iostream>	
 
 // 全局的watcher观察器	zkserver给zkclient的通知	
 void global_watcher(zhandle_t *zh, int type, int state, const char *path, void *watcherctx)	
{	
 	if (type == ZOO_SESSION_EVENT) // 回调的消息类型是和会话相关的消息类型
  {
    if (state == ZOO_CONNECTED_STATE)// zkclient和zkserver连接成功
    {
      sem_t *sem = (sem_t*)zoo_get_context(zh);	
     	sem_post(sem);	
    }
  }
}	

Zkclient::Zkclient():m_zhandle(nullptr)
{
  
}
Zkclient::~Zkclient()
{
  if(m zhandle != nullptr)
    zookeeper_close(m zhandle);//关闭句柄，释放资源 MySQL_conn
}

//连接zkserver
void zkclient::start()
{
  std::string host = MprpcApplication::GetInstance().Getconfig().Load("zookeeperip");
  std::string port = MprpcApplication::GetInstance().Getconfig().Load("zookeeperport");
  std::string connstr =host +":"+ port;

  m_zhandle = zookeeper_init(connstr.c_str(),global watcher, 30080, nullptr, nullptr, 0);
  if(nullptr == m_zhandle)
  {
    std::cout<<"zookeeper初始化发生错误!"<< std::endl;
    exit(EXIT_FAILURE);
  }
  sem_t sem;
  sem_init(&sem,0,0);
  zoo_set_context(m_zhandle,&sem);

  sem_wait(&sem);
  std::cout<<"zookeeper 初始化成功!"<< std::endl;
}

/*
zookeeper_mt: 多线程版本
zookeeper的API客户端程序提供了三个线程: 
1.API调用线程
2.网络I/O线程 pthread_create poll 
3.watcher回调线程
*/

void Zkclient::Create(const char *path, const char *data, int datalen, int state)
{
  char path_buffer[128];
  int bufferlen = sizeof(path_buffer); int flag;
  /先判断path表示的znode节点是否存在，如果存在，就不再重复创建了 
  flag = zoo_exists(m_zhandle,path, 0, nullptr); 
  if (ZNONODE == flag) // 表示path的znode节点不存在
  {
    //创建指定path的znode节点
    flag = zoo_create(m_zhandle, path, data, datalen, &ZO0_OPEN_ACL_UNSAFE, state, path_buffer, bufferlen); 
    if (flag == ZOK)
    {
       std::cout << "Z节点创建成功，路径:" << path << std::endl;
    }
    else
    {
        std::cout << "flag:"<< flag << std::endl;
        std::cout << "znode 创建失败， path:" << path << std::endl; 
        exit(EXIT_FAILURE);
    }
  }
}

// 根据指定的path，获取znode节点的值
std::string zkclient::GetData(const char *path)
{
  char buffer[64];
  int bufferlen = sizeof(buffer);
  int flag = zoo_get(m_zhandle,path, 0, buffer, &bufferlen, nullptr); 
  if (flag != ZOK)
  {
    std::cout << "Z节点创建失败，路径:" << path << std::endl; 
    return "";
  }
else
  return buffer;
}
