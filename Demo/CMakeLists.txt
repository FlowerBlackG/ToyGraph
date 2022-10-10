#[[
    Demo 项目构建文件。
    created on 2022.10.10
]]

add_executable(
    Demo
    main.cpp

    ${PROJECT_SOURCE_DIR}/ToyGraph/lib/src/glad.c
)

target_include_directories(
    Demo PUBLIC
    ${PROJECT_SOURCE_DIR}/ToyGraph/include
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/ToyGraph/lib/include
)


target_link_libraries(
    Demo 
    ToyGraph
)
