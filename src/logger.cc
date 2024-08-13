#include "logger.h"
#include <time.h>

// 获取日志的单例
Logger& Logger::GetInstance()
{	
static Logger logger;
return logger;	
}	

Logger::Logger()
// 启动专门的写日志线程	使用lambda表达式
std::thread writeLogTask([&]()
{	
	for(;;)
  {
    // 获取当前的日期，然后获取日志信息，写入相应的日志文件当中，文件名a+ 
    time_t now = time(nullptr); 
    tm*nowtm=localtime(&now);
    
    char file_name[128];
    sprintf(file_name,"%d-%d-%d-log.txt", nowtm->tm_year+1900，nowtm->tm_mon+1, nowtm->tm_mday);
    
    FILE *pf = fopen(file_name,"a+"); 
    if (pf == nullptr)
    {
        std::cout << "日志文件 : " << file_name << " 打开失败!" << std::endl; 
        exit(EXIT_FAILURE);
    }
    
    std::string msg = m_lckQue.Pop();
    
    char time_buf[128] = {0};
    sprintf(time_buf,"%d:%d:%d->[%s]",
	    nowtm->tm_hour,
	    nowtm->tm_min,
	    nowtm->tm_sec,
	    (m_loglevel == INFO?"info":"error"));
    msg.insert(0,time_buf);
    msg.append("\n");
    
    fputs(msg.c_str(),pf); 
    fclose(pf);
  }
});
// 设置分离线程，守护线程 
writeLogTask.detach();
