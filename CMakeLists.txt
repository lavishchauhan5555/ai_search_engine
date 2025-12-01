cmake_minimum_required(VERSION 3.10)
project(searchengine)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SOURCES "src/*.cpp")

# Add executable
add_executable(searchengine ${SOURCES})

# Link libcurl
find_package(CURL REQUIRED)
target_include_directories(searchengine PRIVATE ${CURL_INCLUDE_DIRS})
target_link_libraries(searchengine PRIVATE ${CURL_LIBRARIES})
