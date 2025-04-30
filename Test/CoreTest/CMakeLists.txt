cmake_minimum_required(VERSION 3.28)
set(TARGET "CoreTest")

file(GLOB CODE_FILES "./*.h" "./*.cpp")
add_executable(${TARGET} ${CODE_FILES})

target_link_libraries(${TARGET} PRIVATE Engine)
set_target_properties(${TARGET} PROPERTIES FOLDER "Test/CoreTest")