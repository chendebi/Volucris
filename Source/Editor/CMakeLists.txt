cmake_minimum_required(VERSION 3.28)

set(TARGET "Editor")

add_executable(${TARGET} "")

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

volucris_engine_code(EditorEntry EDITOR_ENTRY_CODE_FILES)
volucris_engine_code(MainWidget MAIN_WIDGET_CODE_FILES)
volucris_engine_code(ContentBrowser CONTENT_BROWSER_CODE_FILES)

target_sources(${TARGET} PRIVATE 
    ${EDITOR_ENTRY_CODE_FILES}
    ${MAIN_WIDGET_CODE_FILES}
    ${CONTENT_BROWSER_CODE_FILES}
)

target_include_directories(${TARGET} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${TARGET} PUBLIC Engine)
set_target_properties(${TARGET} PROPERTIES FOLDER "")