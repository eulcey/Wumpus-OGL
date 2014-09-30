cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

project(wumpus_ogl)

FILE(GLOB SOURCE_FILES src/*.cpp)
FILE(GLOB GRAPH_FILES deps/scenegraph/*.cpp)
FILE(GLOB RENDER_FILES deps/rendering/*.cpp)
FILE(GLOB UTIL_FILES deps/utils/*.cpp)
FILE(GLOB INPUT_FILES deps/input/*.cpp)
FILE(GLOB OBJECT_FILES src/objects/*.cpp)
FILE(GLOB JSON_FILES deps/jsoncpp/*.cpp)

find_package(OpenGL REQUIRED)

add_executable(
    wumpus_ogl
    ${SOURCE_FILES}
    deps/lodepng/lodepng.cpp
    deps/mathcore/MathCore.cpp
    ${UTIL_FILES}
    ${GRAPH_FILES}
    ${RENDER_FILES}
    ${INPUT_FILES}
    ${OBJECT_FILES}
    ${JSON_FILES}
    )

include_directories(deps/mathcore)
include_directories(/usr/include/GLFW)
include_directories(deps/lodepng)
include_directories(deps/scenegraph)
include_directories(deps/rendering)
include_directories(deps/utils)
include_directories(deps/input)
include_directories(src/objects)
include_directories(deps/jsoncpp)

target_link_libraries(
    wumpus_ogl
    glfw
    GL
    GLEW
    )
