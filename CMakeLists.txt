# 项目名称
project (quick_service)
# CMake最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 头文件目录
include_directories(include)
# 查找./src目录下的所有源文件，保存到DIR_SRCS变量
aux_source_directory(./src DIR_SRCS)
# 用C++11
add_definitions(-std=c++11)
# 指定生成目标文件
#add_executable(quick_service ${DIR_SRCS} )
#add_executable(quick_service ${DIR_SRCS} src/main.cpp include/conn.h src/conn.cpp src/service_mgr.cpp src/work_mgr.cpp include/work_mgr.h)


set(CMAKE_BUILD_TYPE "Debug")
#protobuf begin

SET(PROTOBUF_DIR /usr/local/)
find_package(Protobuf REQUIRED)

set(PROTO proto)
file(GLOB PROTO
        /proto/*.cc
        /proto/*.h
        )
include_directories(proto/include)
aux_source_directory(./proto/protoc DIR_SRCS)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)    #链接库文件
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lprotobuf  -std=c++11  -lpthread ")
# protobuf end
add_executable(quick_service ${DIR_SRCS} ${PROTO})

# 库文件
find_package (Threads)


target_link_libraries (quick_service ${CMAKE_THREAD_LIBS_INIT}   ${PROTOBUF_LIBRARIES}  protobuf )

target_include_directories(quick_service
        PUBLIC
        ${PROTOBUF_INCLUDE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR})