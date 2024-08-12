#pragma once
#include "lockqueue.h"
#include <string>
enum LogLevel
{
  INFO,//普通信息
  ERROR,//错误信息
};
// Mprpc框架提供的日志系统
class Logger
{
public:
  // 设置日志级别
  void setLogLevel(LogLevel level);
  // 写日志
  void Log(std::string msg);
private:
  // 记录日志级别
  int m_loglevel;
  //日志缓冲队列
  LockQueue<std::string> m_lckque;

  Logger();
  Logger(const Logger&)= delete;
  Logger(Logger&&)=delete;
};
