cmake_minimum_required(VERSION 3.12)
project(CapsFactory)

set(CMAKE_CXX_STANDARD 11)

add_executable(CapsFactory main.cpp cJSON.c MsgDefine.cpp MsgDefine.h MsgGroup.cpp MsgGroup.h FieldDefine.cpp FieldDefine.h Common.cpp Common.h clargs.h clargs.cc)