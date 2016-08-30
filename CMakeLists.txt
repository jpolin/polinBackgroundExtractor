cmake_minimum_required(VERSION 2.8)

# For all projects/libs
SET(BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${BASE_DIR})
link_directories(${BASE_DIR})

# Debugging make scheme
#SET(CMAKE_VERBOSE_MAKEFILE ON)

# Projects/libs
add_subdirectory(${BASE_DIR}/extractor)
add_subdirectory(${BASE_DIR}/server)
add_subdirectory(${BASE_DIR}/wrapper)


