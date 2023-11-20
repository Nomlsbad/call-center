include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(${TEST_NAME}
        CallDetailRecordTest.cpp
        ${APP_SOURCES}
)

target_include_directories(${TEST_NAME}
        PUBLIC
        ${APP_INCLUDE_PATH}
)

target_link_libraries(${TEST_NAME}
        GTest::gtest
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${TEST_NAME})