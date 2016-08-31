cmake_minimum_required(VERSION 2.8)


project( bgExtractor )


find_package( OpenCV REQUIRED )

# Create library that others can access
add_library(bgExtractor SHARED bgExtractor.cpp)

target_link_libraries( bgExtractor ${OpenCV_LIBS} )



