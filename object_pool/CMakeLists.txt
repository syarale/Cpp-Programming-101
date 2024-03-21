cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_CXX_STANDARD 11)

find_package(GTest REQUIRED)

add_executable(object_pool_test object_pool_test.cc)
target_link_libraries(object_pool_test GTest::GTest GTest::Main)