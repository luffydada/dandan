#***************************************************************************
#    File name:        Makefile
#    Description:
#    Create date:      2016-11-20 11:30
#    Modified date:    2016-11-20 11:30
#    Version:          V1.0
#    Author:           wangying
#**************************************************************************/
.PHONY: cat prepare all app lib clean
DD_ARM ?= no
DD_ROOT_DIR ?= $(shell pwd)
DD_OUTPUT_DIR ?= $(DD_ROOT_DIR)/output

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

	DD_BUILD_DIR ?= $(DD_OUTPUT_DIR)/arm/bulid
else
	DD_CC := gcc
	DD_CXX := g++

	DD_CFLAGS += -I/usr/include
	DD_CPPFLAGS += -I/usr/include

	DD_LDFLAGS += -L/usr/lib -L/usr/lib/x86_64-linux-gnu

	DD_BUILD_DIR ?= $(DD_OUTPUT_DIR)/pc/bulid
endif

DD_DIRS := kernel server type
DD_SRCS := $(wildcard server/*.cpp) \
		$(wildcard kernel/*.cpp) \
		$(wildcard type/*.cpp)
DD_SRCS_C := $(wildcard server/*.c) \
		$(wildcard kernel/*.c) \
		$(wildcard type/*.c)

DD_OBJS := $(patsubst %.cpp, $(DD_OUTPUT_DIR)/%.o, $(DD_SRCS))
DD_OBJS += $(patsubst %.c, $(DD_OUTPUT_DIR)/%.o, $(DD_SRCS_C))
DD_DEPS := $(patsubst %.cpp, $(DD_OUTPUT_DIR)/%.d, $(DD_SRCS))
DD_DEPS += $(patsubst %.c, $(DD_OUTPUT_DIR)/%.d, $(DD_SRCS_C))

TARGET_LIB := libdandan.so

OBJECTS_APP_SERVER := ddServerTest.o
OBJECTS_APP_CLIENT := ddClientTest.o
TARGET_APP_SERVER := ddServerTest
TARGET_APP_CLIENT := ddClientTest
TARGET_APP := $(TARGET_APP_SERVER) $(TARGET_APP_CLIENT)

cat :
	@echo "DD_ROOT_DIR"=$(DD_ROOT_DIR)
	@echo "DD_OUTPUT_DIR"=$(DD_OUTPUT_DIR)
	@echo "DD_BUILD_DIR"=$(DD_BUILD_DIR)
	@echo "DD_SRCS"=$(DD_SRCS)
	@echo "DD_SRCS_C"=$(DD_SRCS_C)
	@echo "DD_OBJS"=$(DD_OBJS)
	@echo "DD_DEPS"=$(DD_DEPS)

prepare : DD_BUILD_DEP_DIR = mkdir -p $(DD_BUILD_DIR)/$(ARG)
prepare : DD_COMPILE_CPP_DEP = $(DD_CXX) -MM $(ARG) -o $(DD_BUILD_DIR)/$(subst .cpp,.d,$(ARG))
prepare : DD_SED_OBJECT = sed -i "1i $(DB_BUILD_DIR)/$(ARG) \\\\" $(DB_BUILD_DIR)/$(DD_DEPS)
prepare : DD_COMPILE_C_DEP = $(DD_CC) -MM $(ARG) -o $(DD_BUILD_DIR)/$(subst .c,.d,$(ARG))
prepare :
	$(foreach ARG, $(DD_DIRS), $(DD_BUILD_DEP_DIR))
	$(foreach ARG, $(DD_SRCS), $(DD_COMPILE_CPP_DEP))
	$(foreach ARG, $(DD_OBJS), $(DD_COMPILE_CPP_DEP))
	$(foreach ARG, $(DD_SRCS_C), $(DD_COMPILE_C_DEP))
	@for j in $(DB_SRC_NOEXT); do sed -i "1i $(DB_BUILD_DIR)/$$j.o \\\\" $(DB_BUILD_DIR)/$$j.d; done
	@for j in $(DB_SRC_NOEXT); do echo '\t$$(DB_CXX) $$(DB_CPPFLAGS) -c $$< -o $$@' >> $(DB_BUILD_DIR)/$$j.d; done

all : lib app
	
lib : $(DD_OBJS)
	$(DD_CXX) -shared -o $(DD_OUTPUT_DIR)/$(TARGET_LIB) $^ $(DD_LDFLAGS) $(DD_LIBS)

-include $(DD_DEPS)

app : server client 

server : $(OBJECTS_APP_SERVER)
	$(DD_CXX) -o $(TARGET_APP_SERVER) $^ $(DD_LDFLAGS) $(DD_LIBS) -ldandan 

client : $(OBJECTS_APP_CLIENT)
	$(DD_CXX) -o $(TARGET_APP_CLIENT) $^ $(DD_LDFLAGS) $(DD_LIBS) -ldandan 

clean:
	-rm $(TARGET_APP) $(TARGET_LIB)
	-rm *.o
