#***************************************************************************
#    File name:        Makefile
#    Description:
#    Create date:      2016-11-20 11:30
#    Modified date:    2016-11-20 11:30
#    Version:          V1.0
#    Author:           wangying
#**************************************************************************/
.PHONY: all test lib prepare cat clean
DD_ARM ?= no
DD_DEBUG ?= yes
DD_ROOT_DIR ?= $(shell pwd)

CROSS_COMPILE = /opt/arm-linux-gnueabihf/gcc-4.9/bin/arm-linux-gnueabihf-

DD_CFLAGS := -Wall
DD_CPPFLAGS := -Wall
ifeq ($(DD_DEBUG), yes)
	DD_CFLAGS := -g -D__DD_DEBUG__
	DD_CPPFLAGS := -g -D__DD_DEBUG__
endif
DD_LIBS := -lglib-2.0 -lgio-2.0
ifeq ($(DD_ARM), yes)
	DD_CC := $(CROSS_COMPILE)gcc
	DD_CXX := $(CROSS_COMPILE)g++

	DD_CFLAGS += -I/opt/arm-linux-gnueabihf/usr/include -I/opt/arm-linux-gnueabihf/usr/include/gio-unix-2.0
	DD_CPPFLAGS += -I/opt/arm-linux-gnueabihf/usr/include -I/opt/arm-linux-gnueabihf/usr/include/gio-unix-2.0

	DD_LDFLAGS := -L/opt/arm-linux-gnueabihf/lib 
	DD_LDFLAGS += -L/opt/arm-linux-gnueabihf/usr/lib

	DD_LIBS += -lgmodule-2.0 -lgobject-2.0 -lz -lffi

	DD_OUTPUT_DIR ?= $(DD_ROOT_DIR)/output/arm
	DD_BUILD_DIR ?= $(DD_OUTPUT_DIR)/bulid
	DD_OUTPUT_INCDIR ?= $(DD_OUTPUT_DIR)/include
else
	DD_CC := gcc
	DD_CXX := g++

	DD_CFLAGS += -I/usr/include -I/usr/include/c++/4.8
	DD_CFLAGS += -I/usr/include/glib-2.0 -I/usr/include/gio-unix-2.0
#	DD_CFLAGS += -I/usr/local/include -I/usr/include/c++/4.8
#	DD_CFLAGS += -I/usr/local/include/glib-2.0 -I/usr/local/include/gio-unix-2.0
	DD_CPPFLAGS += -I/usr/include -I/usr/include/c++/4.8
	DD_CPPFLAGS += -I/usr/include/glib-2.0 -I/usr/include/gio-unix-2.0
#	DD_CPPFLAGS += -I/usr/local/include -I/usr/include/c++/4.8
#	DD_CPPFLAGS += -I/usr/local/include/glib-2.0 -I/usr/local/include/gio-unix-2.0

	DD_LDFLAGS += -L/usr/lib -L/usr/lib/x86_64-linux-gnu

	DD_OUTPUT_DIR ?= $(DD_ROOT_DIR)/output/pc
	DD_BUILD_DIR ?= $(DD_OUTPUT_DIR)/bulid
	DD_OUTPUT_INCDIR ?= $(DD_OUTPUT_DIR)/include
endif

DD_DIRS := kernel service type utility
DD_SRCS := $(wildcard service/*.cpp) \
		$(wildcard kernel/*.cpp) \
		$(wildcard type/*.cpp) \
		$(wildcard utility/*.cpp)
DD_SRCS_C := $(wildcard service/*.c) \
		$(wildcard kernel/*.c) \
		$(wildcard type/*.c) \
		$(wildcard utility/*.c)

DD_OBJS := $(patsubst %.cpp, $(DD_BUILD_DIR)/%.o, $(DD_SRCS))
DD_OBJS += $(patsubst %.c, $(DD_BUILD_DIR)/%.o, $(DD_SRCS_C))
DD_DEPS := $(patsubst %.cpp, $(DD_BUILD_DIR)/%.d, $(DD_SRCS))
DD_DEPS += $(patsubst %.c, $(DD_BUILD_DIR)/%.d, $(DD_SRCS_C))

TARGET_LIB := libdandan.so

all : lib test
cat :
	@echo "DD_ROOT_DIR"=$(DD_ROOT_DIR)
	@echo "DD_OUTPUT_DIR"=$(DD_OUTPUT_DIR)
	@echo "DD_BUILD_DIR"=$(DD_BUILD_DIR)
	@echo "DD_SRCS"=$(DD_SRCS)
	@echo "DD_SRCS_C"=$(DD_SRCS_C)
	@echo "DD_OBJS"=$(DD_OBJS)
	@echo "DD_DEPS"=$(DD_DEPS)

prepare: DD_SRC_NOEXT := $(basename $(DD_SRCS))
prepare: DD_SRC_C_NOEXT := $(basename $(DD_SRCS_C))
prepare :
	for i in $(DD_DIRS); do mkdir -p $(DD_OUTPUT_INCDIR)/$$i; done
	for i in $(DD_DIRS); do mkdir -p $(DD_BUILD_DIR)/$$i; done
	for i in $(DD_SRC_NOEXT); do $(DD_CXX) -MM $(DD_ROOT_DIR)/$$i.cpp -o $(DD_BUILD_DIR)/$$i.d; done
	for j in $(DD_SRC_NOEXT); do sed -i "1i $(DD_BUILD_DIR)/$$j.o \\\\" $(DD_BUILD_DIR)/$$j.d; done
	for j in $(DD_SRC_NOEXT); do echo '\t$$(DD_CXX) $$(DD_CPPFLAGS) -fPIC -c $$< -o $$@' >> $(DD_BUILD_DIR)/$$j.d; done
	for i in $(DD_SRC_C_NOEXT); do $(DD_CC) -MM $(DD_ROOT_DIR)/$$i.c -o $(DD_BUILD_DIR)/$$i.d; done
	for j in $(DD_SRC_C_NOEXT); do sed -i "1i $(DD_BUILD_DIR)/$$j.o \\\\" $(DD_BUILD_DIR)/$$j.d; done
	for j in $(DD_SRC_C_NOEXT); do echo '\t$$(DD_CC) $$(DD_CPPFLAGS) -fPIC -c $$< -o $$@' >> $(DD_BUILD_DIR)/$$j.d; done
	echo make depend sucess.
	
lib : $(DD_OBJS)
	$(DD_CXX) $(DD_CPPFLAGS) -shared -o $(DD_OUTPUT_DIR)/$(TARGET_LIB) $^ $(DD_LDFLAGS) $(DD_LIBS)

-include $(DD_DEPS)

test : DD_LDFLAGS += -L$(DD_OUTPUT_DIR)
test : DD_LIBS += -ldandan
test : ddServerTest.cpp ddClientTest.cpp
	$(DD_CXX) $(DD_CPPFLAGS) ddServerTest.cpp -o $(DD_OUTPUT_DIR)/ddServer $(DD_LDFLAGS) $(DD_LIBS) 
	$(DD_CXX) $(DD_CPPFLAGS) ddClientTest.cpp -o $(DD_OUTPUT_DIR)/ddClient $(DD_LDFLAGS) $(DD_LIBS) 

clean:
	-rm -R $(DD_OUTPUT_DIR)
	-rm $(TARGET_APP) $(TARGET_LIB)
	-rm *.o
