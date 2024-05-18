cmake_minimum_required(VERSION 3.2)
project(GLUT_Project)

if (POLICY CMP0072)
  set(OpenGL_GL_PREFERENCE LEGACY)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-finput-charset=UTF-8")  
set(CMAKE_C_FLAGS "-finput-charset=UTF-8")  
# set (CMAKE_EXE_LINKER_FLAGS "-static")

find_package(OpenGL REQUIRED)

# 如果是linux平台，则添加特定的编译选项或链接库
if(UNIX)
    find_library(GLUT_glut_LIBRARY NAMES glut freeglut
        PATHS
        ${OPENGL_LIBRARY_DIR}
        ${GLUT_ROOT_PATH}/Release
    )
    set(GLUT_LIBRARY glut)
elseif(WIN32)
    include_directories(${PROJECT_SOURCE_DIR}/../External/glut-3.7.6-bin)
    set(GLUT_LIBRARY ${PROJECT_SOURCE_DIR}/../External/glut-3.7.6-bin/glut32.lib)
endif()

# 添加头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(STB_LIB STATIC
    ${PROJECT_SOURCE_DIR}/src/stb_image/stb_image.cpp
)

add_library(CUSTOM_MATH_LIB STATIC
${PROJECT_SOURCE_DIR}/src/CusMath.cpp
)

# 生成可执行文件
add_executable(${PROJECT_NAME} 
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/src/LineGeneration.cpp
    ${PROJECT_SOURCE_DIR}/src/GLUTCallback.cpp
    ${PROJECT_SOURCE_DIR}/src/Button.cpp
    ${PROJECT_SOURCE_DIR}/src/PolygonScanConversion.cpp
    ${PROJECT_SOURCE_DIR}/src/CircleGenerator.cpp
)

# 链接操作
target_link_libraries(${PROJECT_NAME}
    PUBLIC
    STB_LIB
    OpenGL::GL
    CUSTOM_MATH_LIB
    ${GLUT_LIBRARY}
)