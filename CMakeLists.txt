cmake_minimum_required(VERSION 3.12)
project(CapsFactory)

set(CMAKE_CXX_STANDARD 11)

add_executable(CapsFactory main.cpp cJSON.c CppMsgDefine.cpp CppMsgDefine.h CppMsgGroup.cpp CppMsgGroup.h CppFieldDefine.cpp CppFieldDefine.h Common.cpp Common.h clargs.h clargs.cc JavaMsgDefine.cpp JavaMsgDefine.h BaseAll.cpp BaseAll.h JavaMsgGroup.cpp JavaMsgGroup.h JavaFieldDefine.cpp JavaFieldDefine.h JSMsgGroup.cpp JSMsgGroup.h JSMsgDefine.cpp JSMsgDefine.h JSFieldDefine.cpp JSFieldDefine.h)