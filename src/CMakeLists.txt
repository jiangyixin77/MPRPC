aux_source_directory(. SRC_LIST)
set(SRC_LIST mprpc_application.cc mprpc_config.cc)
add_library(mprpc SHARED ${SRC_LIST})
