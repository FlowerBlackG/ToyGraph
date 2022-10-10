#[[
    ToyGraph 引擎构建文件。
    创建于 2022年10月10日
]]

add_library(
    ToyGraph
    components/ToyGraph/Actor/Actor.cpp
    components/ToyGraph/Camera/Camera.cpp
    components/ToyGraph/Engine/Engine.cpp
    components/ToyGraph/Texture/Texture.cpp
    components/ToyGraph/Shader/Shader.cpp

    lib/src/glad.c
)

target_include_directories(
    ToyGraph PUBLIC
    ${PROJECT_SOURCE_DIR}/ToyGraph/include
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/ToyGraph/lib/include
)


target_link_libraries(
    ToyGraph
    ${PROJECT_SOURCE_DIR}/ToyGraph/lib/lib/glfw3.lib
)
