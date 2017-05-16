LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/release/lib)

ADD_EXECUTABLE(dd_client ddClientTest.cpp)
ADD_EXECUTABLE(dd_dae ddServerTest.cpp)
TARGET_LINK_LIBRARIES(dd_client dandan)
TARGET_LINK_LIBRARIES(dd_dae dandan)

# 设置输出目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/release/bin)

ADD_DEPENDENCIES(dd_client dandan_shared)
ADD_DEPENDENCIES(dd_dae dandan_shared)

