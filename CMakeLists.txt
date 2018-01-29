cmake_minimum_required(VERSION 2.6)
project(MedianFilterRealTime)
set(PROJECT_BRIEF "MedianFilterRealTime")
set(CMAKE_BUILD_TYPE DEBUG)
set(GCC_COMPILE_FLAGS "-std=c++11 -Wall -O3 -g")
set(GCC_LINK_FLAGS "")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_LINK_FLAGS}")
SET ( CF_LIBRARY_LINK_FLAGS "/OPT:NOREF")

#opencl
set(OPENCL_INC_DIR "C:\\Program Files (x86)\\AMD APP SDK\\3.0\\include" CACHE PATH  "path to opencl include directory")
set(OPENCL_LIB_DIR "C:\\Program Files (x86)\\AMD APP SDK\\3.0\\lib\\x86_64" CACHE PATH "path to opencl lib directory")
include_directories(${OPENCL_INC_DIR})
link_directories(${OPENCL_LIB_DIR})

#ffmpeg
set(FFMPEG_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/ffmpeg/include CACHE PATH  "path to ffmpeg include directory")
set(FFMPEG_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/ffmpeg/lib CACHE PATH "path to ffmpeg lib directory")
include_directories(${FFMPEG_INC_DIR})
link_directories(${FFMPEG_LIB_DIR})

#Glew
set(GLEW_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/glew-2.1.0/include  CACHE PATH  "path to Glew include directory")
set(GLEW_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/glew-2.1.0/lib/Release/x64  CACHE PATH  "path to Glew include directory")
include_directories(${GLEW_INC_DIR})
link_directories(${GLEW_LIB_DIR})

#glfw
set(GLFW_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/glfw-3.2.1/build/install/include  CACHE PATH  "path to glfw include directory")
set(GLFW_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/glfw-3.2.1/build/install/lib  CACHE PATH  "path to glfw include directory")
include_directories(${GLFW_INC_DIR})
link_directories(${GLFW_LIB_DIR})


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(src)

