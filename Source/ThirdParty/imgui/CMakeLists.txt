
set(TARGET "imgui")

file(GLOB BACKEND_CODE_FILES "./include/imgui/backends/*.h" "./include/imgui/backends/*.cpp")
file(GLOB SOURCE_CODE_FILES "./source/*.h" "./source/*.cpp")

set(SRC_LIST
    ${SOURCE_CODE_FILES}
	${BACKEND_CODE_FILES}
)

add_library(${TARGET} STATIC ${SRC_LIST})
target_compile_options(${TARGET} PRIVATE "/utf-8")

target_include_directories(${TARGET} PUBLIC "./include" "./include/backends" "./include/imgui")

if (INDEPENDENT_VOLUCRIS)
	set_target_properties(${TARGET} PROPERTIES FOLDER "ThirdParty")
else()
	set_target_properties(${TARGET} PROPERTIES FOLDER "Engine/ThirdParty")
endif()