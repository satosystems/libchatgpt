cmake_minimum_required(VERSION 3.26)
project(libchatgpt)

set(CMAKE_CXX_STANDARD 23)

find_package(CURL REQUIRED)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/nlohmann/json/include" ${CURL_INCLUDE_DIR})

add_library(chatgpt library.cpp)
target_link_libraries(chatgpt ${CURL_LIBRARIES})
