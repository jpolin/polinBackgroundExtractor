cmake_minimum_required(VERSION 2.8)

project( bgExtractor )

find_package( OpenCV REQUIRED )

# Create library that others can access
add_library(bgExtractor SHARED bgExtractor.cpp)

target_link_libraries( bgExtractor ${OpenCV_LIBS} )

# Make an executable that links to the library
project(extractor_main)
add_executable(extractor_main extractor_main.cpp)
target_link_libraries(extractor_main bgExtractor)




