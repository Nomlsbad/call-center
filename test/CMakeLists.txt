add_executable(${TEST_NAME}
        CallDetailRecordTest.cpp
)

target_include_directories(${TEST_NAME}
        PUBLIC
        ${APP_INCLUDE_PATH}
)

target_link_libraries(${TEST_NAME}
        GTest::gtest
        GTest::gtest_main
        ${APP_NAME}
)

include(GoogleTest)
gtest_discover_tests(${TEST_NAME})