INCLUDE_DIRECTORIES(../src)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/release/lib)

ADD_EXECUTABLE(xmlParse parse.cpp)
ADD_EXECUTABLE(xmlGenerate generate.cpp)
TARGET_LINK_LIBRARIES(xmlParse tinyxml)
TARGET_LINK_LIBRARIES(xmlGenerate tinyxml)

# 设置输出目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/release/bin)

ADD_DEPENDENCIES(xmlParse tinyxml)
ADD_DEPENDENCIES(xmlGenerate tinyxml)
