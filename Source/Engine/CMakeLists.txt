cmake_minimum_required(VERSION 3.28)

option(BUILD_WITH_EDITOR ON)

if (INDEPENDENT_VOLUCRIS)
    set(TARGET "Engine")
else()
    set(TARGET "Volucris")
endif()

add_library(${TARGET} STATIC "")

function(volucris_engine_code FOLDER_NAME OUTPUT_LIST)
    file(GLOB PUBLIC_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Public/Engine/${FOLDER_NAME}/*.h)
    file(GLOB PRIVATE_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/${FOLDER_NAME}/*.h)
    file(GLOB PRIVATE_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/${FOLDER_NAME}/*.cpp)
    source_group("${FOLDER_NAME}/Public" FILES ${PUBLIC_HEADER_FILES})
    source_group("${FOLDER_NAME}" FILES ${PRIVATE_HEADER_FILES} ${PRIVATE_SOURCE_FILES})

    set(ALL_FILES 
        ${PUBLIC_HEADER_FILES}
        ${PRIVATE_HEADER_FILES}
        ${PRIVATE_SOURCE_FILES}
    )

    set(${OUTPUT_LIST} ${ALL_FILES} PARENT_SCOPE)
endfunction()

volucris_engine_code(Core CORE_CODE_FILES)
volucris_engine_code(Application APPLICATION_CODE_FILES)
volucris_engine_code(Renderer RENDERER_CODE_FILES)
volucris_engine_code(Renderer/OpenGL RENDERER_OPENGL_CODE_FILES)
volucris_engine_code(Scene SCENE_CODE_FILES)
volucris_engine_code(Resource RESOURCE_CODE_FILES)

target_sources(${TARGET} PRIVATE 
    ${CORE_CODE_FILES}
    ${APPLICATION_CODE_FILES}
    ${RENDERER_CODE_FILES}
    ${RENDERER_OPENGL_CODE_FILES}
    ${SCENE_CODE_FILES}
    ${RESOURCE_CODE_FILES}
)

set_target_properties(${TARGET} PROPERTIES FOLDER "Volucris")
target_link_libraries(${TARGET} PUBLIC fmt glm imgui glad rapidjson)
target_link_libraries(${TARGET} PRIVATE spdlog_header_only glfw assimp)
target_include_directories(${TARGET} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/Public/Engine
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_definitions(${TARGET} PRIVATE VOLUCRIS_ENGINE_ROOT="${VOLUCRIS_ENGINE_ROOT}")
target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Public)

if (BUILD_WITH_EDITOR)
    target_compile_definitions(${TARGET} PRIVATE WITH_EDITOR)
endif()