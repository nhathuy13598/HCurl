cmake_minimum_required(VERSION 3.10)
project(HCurl)

find_package(CURL REQUIRED)

add_executable(example
                example.cpp
                HCurl.cpp
                CurlHeader.cpp
                MultiHCurl.cpp)

include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(example ${CURL_LIBRARY})
target_compile_features(example PUBLIC cxx_std_17)