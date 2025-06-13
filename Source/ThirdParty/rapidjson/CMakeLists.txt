CMAKE_MINIMUM_REQUIRED(VERSION 3.28)

add_library(rapidjson INTERFACE)

target_include_directories(rapidjson INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)