#告知当前使用的是交叉编译方式，必须配置
SET(CMAKE_SYSTEM_NAME Linux)

#指定C交叉编译器,必须配置 
SET(CMAKE_C_COMPILER "/opt/arm-linux-gnueabihf/gcc-4.9/bin/arm-linux-gnueabihf-gcc")

#指定C++交叉编译器
SET(CMAKE_CXX_COMPILER "/opt/arm-linux-gnueabihf/gcc-4.9/bin/arm-linux-gnueabihf-c++")

#指定交叉编译环境安装目录...
SET(CMAKE_FIND_ROOT_PATH "/opt/arm-linux-gnueabihf")

#从来不在指定目录下查找工具程序
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

#只在指定目录下查找库文件
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)

#只在指定目录下查找头文件
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

