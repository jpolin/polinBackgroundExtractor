cmake_minimum_required(VERSION 2.8)

# Name Project
project(server_main)

# Create executable
add_executable(server_main server.cpp server_main.cpp)

# Connect to cpprestsdk
include_directories(cpprestsdk/Release/include)
find_library(CPPREST_LIB cpprest HINTS ${BASE_DIR}/server/cpprestsdk/Release/build/Binaries)
target_link_libraries(server_main ${CPPREST_LIB})

# Connect to extractor
include_directories(${BASE_DIR}/extractor)
target_link_libraries(server_main bgExtractor ssl crypto boost_system)
