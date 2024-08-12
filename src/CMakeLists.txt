#aux_source_directory(. SRC_LIST)
set(SRC_LIST 
    mprpc_application.cc 
    mprpc_config.cc 
    rpc_header.pb.cc 
    rpc_provider.cc
    mprpc_channel.cc
    mprpc_controller.cc)
add_library(mprpc ${SRC_LIST})
#依赖库是有顺序的！！！
target_link_libraries(mprpc muduo_net muduo_base pthread)  
