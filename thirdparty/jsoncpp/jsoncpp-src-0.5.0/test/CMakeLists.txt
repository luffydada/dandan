INCLUDE_DIRECTORIES(..)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/release/lib)

ADD_EXECUTABLE(jsonParse jsonParse.cpp)
ADD_EXECUTABLE(jsonGenerate jsonGenerate.cpp)
TARGET_LINK_LIBRARIES(jsonGenerate json)
TARGET_LINK_LIBRARIES(jsonParse json)

# 设置输出目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/release/bin)

ADD_DEPENDENCIES(jsonGenerate json)
ADD_DEPENDENCIES(jsonParse json)
