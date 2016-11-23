#***************************************************************************
#    File name:        Makefile
#    Description:
#    Create date:      2016-11-20 11:30
#    Modified date:    2016-11-20 11:30
#    Version:          V1.0
#    Author:           wangying
#**************************************************************************/
.PHONY: all app lib prepare cat clean
DD_ARM ?= no
DD_ROOT_DIR ?= $(shell pwd)

CROSS_COMPILE = /opt/arm-linux-gnueabihf/gcc-4.9/bin/arm-linux-gnueabihf-

DD_CFLAGS := -g -Wall -W -fPIC
DD_CPPFLAGS := -g -Wall -W -fPIC
DD_LIBS := -lglib-2.0 -lgio-2.0
ifeq ($(DD_ARM), yes)
	DD_CC := $(CROSS_COMPILE)gcc
	DD_CXX := $(CROSS_COMPILE)g++

	DD_CFLAGS += -I/opt/arm-linux-gnueabihf/usr/include
	DD_CPPFLAGS += -I/opt/arm-linux-gnueabihf/usr/include

	DD_LDFLAGS := -L/opt/arm-linux-gnueabihf/lib 
	DD_LDFLAGS += -L/opt/arm-linux-gnueabihf/usr/lib

	DD_OUTPUT_DIR ?= $(DD_ROOT_DIR)/output/arm
	DD_BUILD_DIR ?= $(DD_OUTPUT_DIR)/bulid
	DD_OUTPUT_INCDIR ?= $(DD_OUTPUT_DIR)/include
else
	DD_CC := gcc
	DD_CXX := g++

	DD_CFLAGS += -I/usr/include
	DD_CFLAGS += -I/usr/include/glib-2.0 -I/usr/include/gio-unix-2.0
#	DD_CFLAGS += -I/usr/local/include
#	DD_CFLAGS += -I/usr/local/include/glib-2.0 -I/usr/local/include/gio-unix-2.0
	DD_CPPFLAGS += -I/usr/include
	DD_CPPFLAGS += -I/usr/include/glib-2.0 -I/usr/include/gio-unix-2.0
#	DD_CPPFLAGS += -I/usr/local/include
#	DD_CPPFLAGS += -I/usr/local/include/glib-2.0 -I/usr/local/include/gio-unix-2.0

	DD_LDFLAGS += -L/usr/lib -L/usr/lib/x86_64-linux-gnu

	DD_OUTPUT_DIR ?= $(DD_ROOT_DIR)/output/pc
	DD_BUILD_DIR ?= $(DD_OUTPUT_DIR)/bulid
	DD_OUTPUT_INCDIR ?= $(DD_OUTPUT_DIR)/include
endif

DD_DIRS := kernel server type
DD_SRCS := $(wildcard server/*.cpp) \
		$(wildcard kernel/*.cpp) \
		$(wildcard type/*.cpp)
DD_SRCS_C := $(wildcard server/*.c) \
		$(wildcard kernel/*.c) \
		$(wildcard type/*.c)

DD_OBJS := $(patsubst %.cpp, $(DD_BUILD_DIR)/%.o, $(DD_SRCS))
DD_OBJS += $(patsubst %.c, $(DD_BUILD_DIR)/%.o, $(DD_SRCS_C))
DD_DEPS := $(patsubst %.cpp, $(DD_BUILD_DIR)/%.d, $(DD_SRCS))
DD_DEPS += $(patsubst %.c, $(DD_BUILD_DIR)/%.d, $(DD_SRCS_C))

TARGET_LIB := libdandan.so

OBJECTS_APP_SERVER := ddServerTest.o
OBJECTS_APP_CLIENT := ddClientTest.o
TARGET_APP_SERVER := ddServerTest
TARGET_APP_CLIENT := ddClientTest
TARGET_APP := $(TARGET_APP_SERVER) $(TARGET_APP_CLIENT)

all : lib app
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
	for j in $(DD_SRC_NOEXT); do echo '\t$$(DD_CXX) $$(DD_CPPFLAGS) -c $$< -o $$@' >> $(DD_BUILD_DIR)/$$j.d; done
	for i in $(DD_SRC_C_NOEXT); do $(DD_CC) -MM $(DD_ROOT_DIR)/$$i.c -o $(DD_BUILD_DIR)/$$i.d; done
	for j in $(DD_SRC_C_NOEXT); do sed -i "1i $(DD_BUILD_DIR)/$$j.o \\\\" $(DD_BUILD_DIR)/$$j.d; done
	for j in $(DD_SRC_C_NOEXT); do echo '\t$$(DD_CC) $$(DD_CPPFLAGS) -c $$< -o $$@' >> $(DD_BUILD_DIR)/$$j.d; done
	echo make depend sucess.
	
lib : $(DD_OBJS)
	$(DD_CXX) -shared -o $(DD_OUTPUT_DIR)/$(TARGET_LIB) $^ $(DD_LDFLAGS) $(DD_LIBS)

-include $(DD_DEPS)

app : server client 

server : DD_LDFLAGS += -L$(DD_OUTPUT_DIR)
server : DD_LIBS += -ldandan
server : $(OBJECTS_APP_SERVER)
	$(DD_CXX) -o $(DD_OUTPUT_DIR)/$(TARGET_APP_SERVER) $^ $(DD_LDFLAGS) $(DD_LIBS) 

client : DD_LDFLAGS += -L$(DD_OUTPUT_DIR)
client : DD_LIBS += -ldandan
client : $(OBJECTS_APP_CLIENT)
	$(DD_CXX) -o $(DD_OUTPUT_DIR)/$(TARGET_APP_CLIENT) $^ $(DD_LDFLAGS) $(DD_LIBS) 

clean:
	-rm $(TARGET_APP) $(TARGET_LIB)
	-rm *.o
