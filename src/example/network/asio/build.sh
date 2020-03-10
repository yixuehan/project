#!/bin/bash

#多个值要用双引号包含！！！！！！！！！！！！！！

#指定生成动态库dynamic  静态库static 可执行目标exec 对应的debug和release方式及JAVA字节码java
targetType=exec

#指定目标列表  如：test  动态库会自动扩展为libtest.so 静态库同理
targets="sync_tcp_server sync_tcp_client async_tcp_client async_tcp_server"

#指定目标依赖
#命名规则  目标_ojbs  目标_libs  如:test_objs  test_libs test_includepath
testmake_objs=
sync_tcp_server_libs="-lboost_system -lpthread"
async_tcp_server_libs="-lboost_system -lpthread"
sync_tcp_client_libs="-lboost_system -lboost_timer -lpthread"
async_tcp_client_libs="-lboost_system -lboost_timer -lpthread"
xxx_include_path=


#下面的代码不要修改!!!!!!!!!!!!!!!!!!!!!!!!

. ${MKHOME}/mkcore.sh

