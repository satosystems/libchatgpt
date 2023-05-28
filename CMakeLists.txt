cmake_minimum_required(VERSION 3.26)
project(libchatgpt)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-O0 -g)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3)
endif()

set(CMAKE_CXX_STANDARD 23)

find_package(CURL REQUIRED)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/nlohmann/json/include" ${CURL_INCLUDE_DIR})

add_library(chatgpt chatgpt.cpp)
target_link_libraries(chatgpt ${CURL_LIBRARIES})
