cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror" CACHE STRING "compile flags" FORCE)

project(wumpus_ogl)

option(USE_ES_SHADER "use es glsl shader instead of core shader" off)

FILE(GLOB SOURCE_FILES src/*.cpp)
FILE(GLOB GRAPH_FILES deps/scenegraph/*.cpp)
FILE(GLOB RENDER_FILES deps/rendering/*.cpp)
FILE(GLOB UTIL_FILES deps/utils/*.cpp)
FILE(GLOB INPUT_FILES deps/input/*.cpp)
FILE(GLOB OBJECT_FILES src/objects/*.cpp)
FILE(GLOB JSON_FILES deps/jsoncpp/*.cpp)
FILE(GLOB AI_FILES src/logic/*.cpp)

find_package(OpenGL REQUIRED)

add_executable(
    wumpus_ogl
    ${SOURCE_FILES}
    ${AI_FILES}
    deps/lodepng/lodepng.cpp
    deps/mathcore/MathCore.cpp
    ${UTIL_FILES}
    ${GRAPH_FILES}
    ${RENDER_FILES}
    ${INPUT_FILES}
    ${OBJECT_FILES}
    ${JSON_FILES}
)

add_subdirectory(libs)

include_directories(
	libs/glfw/include/GLFW/
	libs/glew/include/
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

include_directories(deps/mathcore)
include_directories(deps/lodepng)
include_directories(deps/scenegraph)
include_directories(deps/rendering)
include_directories(deps/utils)
include_directories(deps/input)
include_directories(src/objects)
include_directories(deps/jsoncpp)
include_directories(src/logic)
include_directories(src)

target_link_libraries(
    wumpus_ogl
    ${ALL_LIBS}
)

if(USE_ES_SHADER)
  set_target_properties(wumpus_ogl PROPERTIES COMPILE_DEFINITIONS "USE_ES_SHADER=1")
endif()