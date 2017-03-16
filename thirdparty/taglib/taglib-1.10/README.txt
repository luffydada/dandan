1.pc端编译
a.请在build目录下使用指令生成makefile文件:cmake ..
b.直接运行make生成目标文件

2.交叉编译:
a.请在build目录下使用指令生成makefile文件:cmake -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_STATIC=ON -DENABLE_STATIC_RUNTIME=ON -DCMAKE_INSTALL_PREFIX=../release ..
b.直接运行make生成目标文件
