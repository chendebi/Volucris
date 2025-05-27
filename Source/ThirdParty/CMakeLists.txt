add_subdirectory(fmt)
set(fmt_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fmt)

# 设置别名确保兼容性
if(NOT TARGET fmt::fmt)
    add_library(fmt::fmt ALIAS fmt-header-only)
endif()

set(SPDLOG_FMT_EXTERNAL_HO ON)
set(fmt_DIR "${CMAKE_CURRENT_SOURCE_DIR}/fmt")
add_subdirectory(spdlog)
add_subdirectory(glad)
add_subdirectory(glm)
add_subdirectory(imgui)
add_subdirectory(glfw)

add_subdirectory(rapidjson)

if (INDEPENDENT_VOLUCRIS)
	set_target_properties(spdlog PROPERTIES FOLDER "ThirdParty")
	set_target_properties(fmt PROPERTIES FOLDER "ThirdParty")
	set_target_properties(glm PROPERTIES FOLDER "ThirdParty")
	set_target_properties(rapidjson PROPERTIES FOLDER "ThirdParty")
else()
	set_target_properties(spdlog PROPERTIES FOLDER "Engine/ThirdParty")
	set_target_properties(fmt PROPERTIES FOLDER "Engine/ThirdParty")
	set_target_properties(glm PROPERTIES FOLDER "Engine/ThirdParty")
	set_target_properties(rapidjson PROPERTIES FOLDER "Engine/ThirdParty")
endif()