# mprpc分布式网络通信框架项目
本项目基于muduo高性能网络库+Protobuf开发

# 技术栈
- 集群和分布式的概念与原理
- RPC的原理与实现
- Protobuf的数据序列化、反序列化
- ZooKeeper分布式一致性协调服务应用与编程
- muduo网络库编程
- conf配置文件读取
- CMake构建项目集成编译环境
- C++11部分新特性

# 集群和分布式的概念与原理
![image](https://github.com/user-attachments/assets/306273c3-7a3a-4c6d-9416-7a66c518eabb)
- 如果使用**单机**服务器会导致：服务器能承受的**并发量**受限于硬件资源；修改任意模块都会导致整个项目代码**重新编译部署**；各模块对硬件**需求不一**（有的模块CPU密集，有的I/O密集）
![image](https://github.com/user-attachments/assets/acf59fe4-960b-4dc5-a899-d1dc04612a41)
- 引入**集群**，每台服务器独立运行一个工程所有模块。并发量提升了，但重新编译部署的问题没有改善，且管理这样的模块根本无需高并发。
![image](https://github.com/user-attachments/assets/dfd384bf-b42c-45fd-afe3-a238088a6ed9)
- 引入**分布式**，把一个工程拆分为多个模块，每个模块独立部署运行在一个服务器上，所有服务器协作提供服务。每台服务器称为一个**节点**，根据节点并发要求可以再做节点模块集群部署。

# 分布式遇到的问题
- 系统的软件模块怎么划分？（各个模块可能会出现重复代码）
- 不同模块间如何访问？（机器1模块如何调用机器2上的模块的一个业务方法呢？）（模块进程1如何调用同一机器的模块进程2里面的一个业务方法呢？）
- 解决方法：把模块间的通信全部封装在**分布式网络通信框架**当中
![image](https://github.com/user-attachments/assets/f3b64982-dc26-44ad-b4f4-8b438627b305)

# RPC通信原理
RPC，即远程过程调用协议
![image](https://github.com/user-attachments/assets/9655a39d-4b7b-4d33-b014-541a20552907)
我们要实现的**分布式网络通信框架**负责处理的范围：图中黄/绿色部分
黄色部分：进行序列化与反序列化
绿色部分：网络部分，寻找RPC服务主机，发起RPC调用请求，响应RPC调用结果，使用muduo网络库和Zookeeper服务配置中心，专门做服务发现

# Protobuf相当于Json好处
- Protobuf用二进制存储节省空间，Json文本存储
- Protobuf不需要存储额外信息，Json用key和value
