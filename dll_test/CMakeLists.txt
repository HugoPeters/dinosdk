cmake_minimum_required(VERSION 3.19)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/env.cmake)

project(DLL_TEST)

file(GLOB_RECURSE PROJ_SRCS *.*)
file(GLOB_RECURSE SDK_SRCS ${base_dir}/src/*.*)
set(SRCS ${PROJ_SRCS} ${SDK_SRCS})

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/project.cmake)