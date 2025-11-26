cmake_minimum_required(VERSION 3.10)
project(AI_SearchEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Source files
file(GLOB SOURCES "src/*.cpp")

# Build executable
add_executable(crawler ${SOURCES})

# Link libraries (we will add cURL later)
find_package(CURL REQUIRED)
target_link_libraries(crawler CURL::libcurl)


find_package(CURL REQUIRED)
target_link_libraries(crawler CURL::libcurl)
