cmake_minimum_required(VERSION 2.8)

# Name Project
project(backgroundExtractionServer)

# Create executable
add_executable(backgroundExtractionServer server.cpp)

# Connect to extractor
include_directories(${BASE_DIR}/extractor)
target_link_libraries(backgroundExtractionServer bgExtractor)
