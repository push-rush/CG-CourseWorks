cmake_minimum_required(VERSION 3.0)
project(GLUT_Project)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-finput-charset=UTF-8")  
set(CMAKE_C_FLAGS "-finput-charset=UTF-8")  
set (CMAKE_EXE_LINKER_FLAGS "-static")

# 添加头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../External/glut-3.7.6-bin)

add_library(STB_LIB STATIC
    ${PROJECT_SOURCE_DIR}/src/stb_image/stb_image.cpp
)

add_library(CUSTOM_MATH_LIB STATIC
    ${PROJECT_SOURCE_DIR}/src/CusMath.c
)

# 生成可执行文件
add_executable(${PROJECT_NAME} 
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/src/Curve.cpp
    ${PROJECT_SOURCE_DIR}/src/GLUTCallback.cpp
)

# 链接操作
target_link_libraries(${PROJECT_NAME}
    PUBLIC
    STB_LIB
    OpenGL::GL
    CUSTOM_MATH_LIB
    ${PROJECT_SOURCE_DIR}/../External/glut-3.7.6-bin/glut32.lib
)