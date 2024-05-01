include(GoogleTest)

find_package(GTest CONFIG REQUIRED)

add_executable("test-arcball" test.cpp)
target_link_libraries("test-arcball" PUBLIC arcball GTest::gtest_main)
target_folder("test-arcball" "Tests")

gtest_discover_tests("test-arcball")
