cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_CXX_STANDARD 17)

find_package(GTest REQUIRED)

add_executable(message_bus_test message_bus_test.cc)
target_link_libraries(message_bus_test GTest::GTest GTest::Main)