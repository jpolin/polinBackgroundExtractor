cmake_minimum_required(VERSION 2.8)

# Name Project
project(backgroundExtractionServer)

# Create executable
add_executable(backgroundExtractionServer server.cpp)

# Connect to cpprestsdk
include_directories(cpprestsdk/Release/include)
find_library(CPPREST_LIB cpprest HINTS ${BASE_DIR}/server/cpprestsdk/Release/build/Binaries)
target_link_libraries(backgroundExtractionServer ${CPPREST_LIB})

# Connect to extractor
include_directories(${BASE_DIR}/extractor)
target_link_libraries(backgroundExtractionServer bgExtractor ssl crypto boost_system)
