
set(TARGET "glad")

set(SRC_LIST
    "src/glad.c"
)

add_library(${TARGET} STATIC ${SRC_LIST})
target_compile_options(${TARGET} PRIVATE "/utf-8")

target_include_directories(${TARGET} PUBLIC "./include")

if (INDEPENDENT_VOLUCRIS)
	set_target_properties(${TARGET} PROPERTIES FOLDER "ThirdParty")
else()
	set_target_properties(${TARGET} PROPERTIES FOLDER "Engine/ThirdParty")
endif()