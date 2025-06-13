
set(TARGET "stb_image")

set(SRC_LIST
    "stb_image.cpp"
)

add_library(${TARGET} STATIC ${SRC_LIST})

target_include_directories(${TARGET} PUBLIC "./include")

if (INDEPENDENT_VOLUCRIS)
	set_target_properties(${TARGET} PROPERTIES FOLDER "ThirdParty")
else()
	set_target_properties(${TARGET} PROPERTIES FOLDER "Engine/ThirdParty")
endif()