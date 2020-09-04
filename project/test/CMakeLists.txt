cmake_minimum_required(VERSION 3.10)

find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_subdirectory(./lib/googletest)
include_directories(lib/googletest/include)
add_executable(Tests test.cpp)

target_link_libraries(Tests gtest gmock gtest_main)

