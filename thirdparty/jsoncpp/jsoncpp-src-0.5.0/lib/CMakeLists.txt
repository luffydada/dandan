INCLUDE_DIRECTORIES(..)
AUX_SOURCE_DIRECTORY(. SRCS)

# 生成lib
ADD_LIBRARY(json SHARED ${SRCS})
ADD_LIBRARY(json_static STATIC ${SRCS})
SET_TARGET_PROPERTIES(json PROPERTIES OUTPUT_NAME "json")
SET_TARGET_PROPERTIES(json_static PROPERTIES OUTPUT_NAME "json")

# 设置动态库的版本号
SET_TARGET_PROPERTIES(json PROPERTIES VERSION 1.1 SOVERSION 1)

# 设置输出目录
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/release/lib)
