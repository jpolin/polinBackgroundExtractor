cmake_minimum_required(VERSION 2.8)

# Name Project
project(bgExtractorWrapper)

# Create executable
add_executable(bgExtractorWrapper wrapper.cpp)

# Connect to extractor
include_directories(${BASE_DIR}/extractor)
target_link_libraries(bgExtractorWrapper bgExtractor)
