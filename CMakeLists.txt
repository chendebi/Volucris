cmake_minimum_required(VERSION 3.28)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(INDEPENDENT_VOLUCRIS true)
    project(Volucris)
else()
    set(INDEPENDENT_VOLUCRIS false)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VOLUCRIS_ENGINE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Binaries/$<CONFIG>)
set(VOLUCRIS_ASSIMP_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Binaries/ThirdParty/assimp)

add_subdirectory(Source/Engine)
add_subdirectory(Source/ThirdParty EXCLUDE_FROM_ALL)

if (${INDEPENDENT_VOLUCRIS})
    add_subdirectory(Source/Editor)
	add_subdirectory(Test/CoreTest)
endif()