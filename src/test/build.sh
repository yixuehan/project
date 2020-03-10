#!/bin/bash

#多个值要用双引号包含！！！！！！！！！！！！！！

#指定生成动态库dynamic  静态库static 可执行目标exec 对应的debug和release方式及JAVA字节码java
targetType=exec

#指定目标列表  如：test  动态库会自动扩展为libtest.so 静态库同理
targets="signal_elapsed testString testgcore testMax class1 testRValue rangeFor \
         testThread thread_calc"

#指定目标依赖
#命名规则  目标_ojbs  目标_libs  如:test_objs  test_libs test_includepath
thread_calc_libs="-lpthread -lboost_timer"
testThread_libs="-lpthread"
testRValue_libs="-lpthread"
class1_objs="class1.o"
testmake_objs="main.o"
testmake_libs="-lboost_system -lpthread -lOpenCL"
logs_libs="-lboost_system -lboost_log -lboost_log_setup -lboost_thread -lpthread"
testString_libs="-lboost_timer"
testSwitchString_libs="-lboost_timer -lfast_string"
testMax_libs="-lboost_timer"
testgcore_libs="-lpthread"

#testAbort_include_path="-I/home/wangkai/erie/local7/include"
xxx_include_path=

signal_elapsed_libs="-lboost_timer"


#下面的代码不要修改!!!!!!!!!!!!!!!!!!!!!!!!

export MKHOME=${PRONAME}/mak
. ${MKHOME}/mkcore.sh

