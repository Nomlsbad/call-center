cmake_minimum_required(VERSION 3.26)
project(call_center LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_SOURCE_DIR}/log4cpp/include)
find_library(Log4Cpp_LIB log4cpp "${CMAKE_SOURCE_DIR}/log4cpp/lib")

add_subdirectory(src)

enable_testing()
add_subdirectory(test)