cmake_minimum_required(VERSION 3.10)
project(HCurl)

find_package(CURL REQUIRED)

add_executable(example
                example.cpp)

include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(example ${CURL_LIBRARY})