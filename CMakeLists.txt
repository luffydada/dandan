CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(dandan)
MESSAGE(STATUS "src dir:" ${PROJECT_SOURCE_DIR})
MESSAGE(STATUS "bin dir:" ${PROJECT_BINARY_DIR})

##########################################
AUX_SOURCE_DIRECTORY(service SRCS)
AUX_SOURCE_DIRECTORY(kernel SRCS)
AUX_SOURCE_DIRECTORY(type SRCS)
AUX_SOURCE_DIRECTORY(utility SRCS)

ADD_DEFINITIONS(-D__DD_DEBUG__)

#OPTION(ARM_COMPILE "Use arm compile" OFF) 
IF(ARM_COMPILE)
	MESSAGE(STATUS "arm compile")
	INCLUDE_DIRECTORIES(/opt/arm-linux-gnueabihf/usr/include /opt/arm-linux-gnueabihf/usr/include/gio-unix-2.0 /opt/arm-linux-gnueabihf/usr/include/gstreamer-1.0)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/thirdparty/taglib/taglib-1.10/release/include)
	LINK_DIRECTORIES(/opt/arm-linux-gnueabihf/lib /opt/arm-linux-gnueabihf/usr/lib)
	LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/thirdparty/taglib/taglib-1.10/release/lib)
ELSE(ARM_COMPILE)
	MESSAGE(STATUS "pc compile")
	INCLUDE_DIRECTORIES(/usr/include/c++/4.8 /usr/include/glib-2.0 /usr/include/gio-unix-2.0 /usr/include/gstreamer-1.0)
	LINK_DIRECTORIES(/usr/lib /usr/lib/x86_64-linux-gnu)
ENDIF(ARM_COMPILE)
# 生成lib
ADD_LIBRARY(dandan_shared SHARED ${SRCS})
#ADD_LIBRARY(dandan_static STATIC ${SRCS})
SET_TARGET_PROPERTIES(dandan_shared PROPERTIES OUTPUT_NAME "dandan")
#SET_TARGET_PROPERTIES(dandan_static PROPERTIES OUTPUT_NAME "dandan")

IF(CMAKE_FIND_ROOT_PATH)
	TARGET_LINK_LIBRARIES(dandan_shared gmodule-2.0 gobject-2.0 z ffi)
ENDIF(CMAKE_FIND_ROOT_PATH)
TARGET_LINK_LIBRARIES(dandan_shared glib-2.0 gio-2.0 gstreamer-1.0 tag)
#TARGET_LINK_LIBRARIES(dandan_static glib-2.0 gio-2.0 gstreamer-1.0 tag)

# 设置动态库的版本号
SET_TARGET_PROPERTIES(dandan_shared PROPERTIES VERSION 1.1 SOVERSION 1)

# 设置输出目录
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/output/lib)

# 安装目录
INSTALL(TARGETS dandan_shared
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	)

#INSTALL(FILES dandan.h service/dd_command.h service/dd_service.h service/dd_devsrvmanager.h service/dd_device.h
#	kernel/dd_app.h kernel/dd_mediadevice.h kernel/dd_process.h kernel/dd_thread.h kernel/dd_timer.h type/dd_type.h 
#	utility/dd_character.h utility/dd_filemanager.h utility/dd_log.h utility/dd_mediaplayer.h utility/dd_playlist.h
#	utility/dd_reg.h DESTINATION include
#	)
INSTALL(FILES dandan.h DESTINATION include)

INSTALL(DIRECTORY service kernel utility type DESTINATION include
	FILES_MATCHING PATTERN "*.h"
	PATTERN "dd-daemon.h" EXCLUDE
	)

##########################################
ADD_SUBDIRECTORY(test)
