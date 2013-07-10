project(mmu CXX)
cmake_minimum_required(VERSION 2.8)

# set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_MODULE_PATH ${mmu_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})
include(MacroOutOfSourceBuild)
macro_ensure_out_of_source_build("${PROJECT_NAME} requires an out of source build.")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(src)

if (UNIX)
  set(CMAKE_CXX_FLAGS "-Wall -std=c++0x")
endif (UNIX)
