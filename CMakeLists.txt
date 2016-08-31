cmake_minimum_required(VERSION 2.8)

#message(STATUS ${CMAKE_BUILD_TYPE})

# For all projects/libs
SET(BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${BASE_DIR})
link_directories(${BASE_DIR})

# Debugging make scheme
#SET(CMAKE_VERBOSE_MAKEFILE ON)

# Use C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Set DEBUG flag for files
if (CMAKE_BUILD_TYPE MATCHES "DEBUG")
    ADD_DEFINITIONS(-DDEBUG)
endif()

# Projects/libs
add_subdirectory(${BASE_DIR}/extractor)
add_subdirectory(${BASE_DIR}/server)
add_subdirectory(${BASE_DIR}/wrapper)


