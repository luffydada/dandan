LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/output/lib)

ADD_EXECUTABLE(ddClient ddClientTest.cpp)
ADD_EXECUTABLE(ddServer ddServerTest.cpp)
TARGET_LINK_LIBRARIES(ddClient dandan)
TARGET_LINK_LIBRARIES(ddServer dandan)

# 设置输出目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/output/bin)

ADD_DEPENDENCIES(ddClient dandan_shared)
ADD_DEPENDENCIES(ddServer dandan_shared)
