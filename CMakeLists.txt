cmake_minimum_required(VERSION 3.26)
project(call_center LANGUAGES CXX)

#C++ compiler
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#Boost
find_package(Boost 1.83 REQUIRED)
set(Boost_USE_MULTITHREADED ON)

#Log4cplus
include(FetchContent)
FetchContent_Declare(
        log4cplus
        GIT_REPOSITORY "https://github.com/log4cplus/log4cplus"
        GIT_TAG REL_2_1_1
)
FetchContent_MakeAvailable(log4cplus)

#Google Tests
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

#App
set(APP_NAME ${PROJECT_NAME}_app)

set (APP_PATH "${PROJECT_SOURCE_DIR}/app")
set (APP_SRC_PATH  "${APP_PATH}/src" )
set (APP_INCLUDE_PATH  "${APP_PATH}/include" )
set (APP_MAIN "${APP_SRC_PATH}/main.cpp")

file (GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS "${APP_SRC_PATH}/*.cpp")
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