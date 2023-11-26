cmake_minimum_required(VERSION 3.26)
project(call_center LANGUAGES CXX)

#C++ compiler
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#Boost
set(Boost_USE_MULTITHREADED ON)

#Log4cplus
include(FetchContent)
FetchContent_Declare(
        log4cplus
        GIT_REPOSITORY "https://github.com/log4cplus/log4cplus"
        GIT_TAG REL_2_1_1
)
FetchContent_MakeAvailable(log4cplus)

#App
set(APP_NAME ${PROJECT_NAME}_app)

set (APP_PATH "${PROJECT_SOURCE_DIR}/app")
set (APP_SRC_PATH  "${APP_PATH}/src" )
set (APP_INCLUDE_PATH  "${APP_PATH}/include" )

file (GLOB APP_SOURCES "${APP_SRC_PATH}/*.cpp")
list(FILTER APP_SOURCES EXCLUDE REGEX "${APP_SRC_PATH}/*/main.cpp")
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