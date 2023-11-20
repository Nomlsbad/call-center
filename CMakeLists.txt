cmake_minimum_required(VERSION 3.26)
project(call_center LANGUAGES CXX)

#C++ compiler
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#Boost
set(Boost_USE_MULTITHREADED ON)

#Log4cpp
include_directories(${CMAKE_SOURCE_DIR}/log4cpp/include)
find_library(Log4Cpp_LIB log4cpp "${PROJECT_SOURCE_DIR}/log4cpp/lib")

#App
set(APP_NAME ${PROJECT_NAME}_app)

set (APP_PATH "${PROJECT_SOURCE_DIR}/app")
set (APP_SRC_PATH  "${APP_PATH}/src" )
set (APP_INCLUDE_PATH  "${APP_PATH}/include" )

file (GLOB APP_SOURCES "${APP_SRC_PATH}/*.cpp")
message(STATUS ${APP_SOURCES} )

add_subdirectory(app)

#Test
set(TEST_NAME ${PROJECT_NAME}_test)

set (TEST_PATH "${PROJECT_SOURCE_DIR}/test")
set (TEST_SRC_PATH ${TEST_PATH})

file (GLOB TEST_SOURCES "${TEST_SRC_PATH}/*.cpp")
message(STATUS ${TEST_SOURCES} )

enable_testing()
add_subdirectory(test)