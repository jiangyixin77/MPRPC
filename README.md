# MPRPC分布式网络通信框架项目
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

# 分布式遇到的问题与分布式网络通信框架的引入
- 系统的软件模块怎么划分？（各个模块可能会出现重复代码）
- 不同模块间访问问题：①机器1模块如何调用机器2上的模块的一个业务方法；②模块进程1如何调用同一机器的模块进程2里面的一个业务方法
- 解决方法：把模块间的通信全部封装在分布式网络通信框架当中
![image](https://github.com/user-attachments/assets/f3b64982-dc26-44ad-b4f4-8b438627b305)

# RPC通信原理
- RPC，即远程过程调用协议
![image](https://github.com/user-attachments/assets/9655a39d-4b7b-4d33-b014-541a20552907)
- 分布式网络通信框架负责处理的范围：图中黄/绿色部分
- 黄色部分：进行序列化与反序列化
- 绿色部分：网络部分，寻找RPC服务主机，发起RPC调用请求，响应RPC调用结果，使用muduo网络库和Zookeeper服务配置中心，专门做服务发现

# muduo网络库编程概述
- 先安装boost和muduo库
- ![image](https://github.com/user-attachments/assets/69ef3a67-dadc-47d4-a0eb-9db5ea17a69b)
- 该原理的特点是一个线程一个事件循环，主反应器负载accept连接，然后通过轮询方式分发到分反应器。连接操作都在分反应器所在线程完成。
- ![image](https://github.com/user-attachments/assets/5e085b38-1743-4a53-ac0f-286823eade96)
- muduo的reactor模型

# muduo网络库示例
- ```/example/muduo_server.cpp```
- 执行```g++ -o server muduo_server.cpp -lmuduo_net -lmuduo_base -lpthread``` 进行编译，生成可执行文件server并执行
- ![image](https://github.com/user-attachments/assets/91efbe99-512e-4f97-98ec-b38e3a1738a6)


# Protobuf序列化/反序列化示例
- ```test/protobuf/test.proto```
- 执行```protoc test.proto --cpp_out=./``` 生成```test.pb.cc```和```test.pb.h```
- ```test/protobuf/main.cc```   进行序列化与反序列化
- 执行```g++ main.cc test.pb.cc -lprotobuf```

# Protobuf底层原理
- ![image](https://github.com/user-attachments/assets/a1f428bd-1514-4048-b1d0-97933b899161)
- ![image](https://github.com/user-attachments/assets/fe3d8d5d-59a5-48ca-926a-4d5c0614621f)
- ![image](https://github.com/user-attachments/assets/8b26539b-3546-4754-8c4c-a7178a668b90)

# 本地服务发布为RPC服务与MPRPC框架基础类设计
- ```example/callee/userservice.cc```
- ```src/mprpc_application.cc```
- ```src/mprpc_application.h```
- ```src/rpc_provider.cc```
- ```src/rpc_provider.h```
- 进行CMake编译，生成```lib/libmprpc.so```以及可执行文件```bin/provider```

# MPRPC框架配置文件加载
- ```src/include/mprpc_config.h```
- ```src/include/mprpc_config.cc```
- ```/bin/test.conf```

# RpcProvider网络服务开发，服务方法发布，分发RPC服务，以及RPC响应回调实现
- ```/src/rpc_provider.cc```
- ```/src/rpc_provider.h```
- ```/src/rpc_header.proto```
- 执行```protoc rpc_header.proto --cpp_out=./```对proto进行编译
- 进行CMake编译            
- ![无标题](https://github.com/user-attachments/assets/96e8c689-725f-424f-97f9-2064ec411886)

# caller端调用RpcChannel
- ```example/caller/calluser_service.cc```
- ```src/include/mprpc_channel.h```

# 实现RPC方法的调用过程
- ![image](https://github.com/user-attachments/assets/471ed19a-fe1b-490c-b83b-407faea284e8)
- ```example/caller/call_user_service.cc``` 
- ```src/include/mprpc_channel.cc```

# MPRPC框架加入注册功能
- ```example/user.proto```
- ```example/callee/user_service.cc```
- ```caller/calluser_service.cc```

# MPRPC框架加入好友管理功能
- ```example/friend.proto``` 
- ```example/callee/friend_service.cc``` 
- ```example/caller/callfriend_service.cc``` 
- ```src/include/mprpc_controller.h```
- ```src/include/mprpc_controller.cc```
- ```src/mprpc_channel.cc```

# logger日志系统与异步日志缓冲队列设计
- ![image](https://github.com/user-attachments/assets/28dcb138-bd5b-4736-a849-0fe27ffc0ebe)
- ```src/include/logger.h```
- ```src/include/lockqueue.h```
- ```src/logger.cc```
- ```src/rpc_provider.cc```

# zookeeper
- zk在分布式环境中应用广泛，优点：分布式环境全局命名服务，服务注册中心，全局分布式锁等
- 配置中心查找相应的服务，进行远程调用(ip:port)  
- 基于服务配置中心的协调功能
- zk存储数据就是一个特殊的文件系统，zk数据通过znode节点组织
- ![image](https://github.com/user-attachments/assets/1d4680a3-2cb2-47f8-9139-8f38acf405ba)
- zk命令：ls看节点，get查节点，create建节点，set改，delete删
- ![image](https://github.com/user-attachments/assets/df7c2442-4676-4814-ad48-f71da74fe7af)
- ![image](https://github.com/user-attachments/assets/85261593-f6c5-4ee7-bb7b-da79ce474816)
- ![image](https://github.com/user-attachments/assets/e6f9d4fd-030a-4fcd-8e22-74fd09bcffc6)
- 永久性节点不会删，删临时性节点
- ![image](https://github.com/user-attachments/assets/62b80d79-5461-4716-8609-88d3c00e5412)

# 封装zk客户端类
- ```src/include/zk_util.h```
- ```src/include/zk_util.cc```
- ```bin/test.conf```

# 总体框架
- ![image](https://github.com/user-attachments/assets/dae76210-e765-455a-8891-735bba118424)
- ![image](https://github.com/user-attachments/assets/d8bc0704-2106-4426-8899-bfa8ce544ff5)
