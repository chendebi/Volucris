set(SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# 递归获取所有着色器文件
file(GLOB_RECURSE SHADER_FILES 
    "${SHADER_DIR}/*.vert"
    "${SHADER_DIR}/*.frag"
)

# 创建分组映射表：文件名 -> 文件列表
foreach(SHADER_FILE ${SHADER_FILES})
    # 获取相对路径（去掉shaders/前缀）
    file(RELATIVE_PATH REL_PATH ${SHADER_DIR} ${SHADER_FILE})
    
    # 提取目录部分和基础名（不含扩展名）
    get_filename_component(DIR_PATH "${REL_PATH}" DIRECTORY)
    get_filename_component(BASE_NAME "${REL_PATH}" NAME_WE)
    
    # 为同名文件创建分组键（格式：目录/基础名）
    if(DIR_PATH)
        set(GROUP_KEY "${DIR_PATH}/${BASE_NAME}")
    else()
        set(GROUP_KEY "${BASE_NAME}")
    endif()
    
    # 添加到分组映射
    list(APPEND ALL_GROUPS ${GROUP_KEY})
    list(APPEND GROUP_${GROUP_KEY} ${SHADER_FILE})
endforeach()

# 去重分组键
list(REMOVE_DUPLICATES ALL_GROUPS)

# 创建虚拟目标并设置分组
set(TARGET Shader)
add_custom_target(${TARGET} SOURCES ${SHADER_FILES})

# 为每个分组创建源组
foreach(GROUP_KEY ${ALL_GROUPS})
    # 转换路径格式（将/替换为\\）
    string(REPLACE "/" "\\" GROUP_NAME "${GROUP_KEY}")
    
    # 设置源组
    source_group("${GROUP_NAME}" FILES ${GROUP_${GROUP_KEY}})
endforeach()

set_target_properties(${TARGET} PROPERTIES FOLDER "Volucris")