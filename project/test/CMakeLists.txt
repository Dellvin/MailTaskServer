cmake_minimum_required(VERSION 3.15)

find_package(Threads REQUIRED)
find_package(Threads)
set(THREADS_PREFER_PTHREAD_FLAG ON)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
add_subdirectory(./lib/googletest)
include_directories(lib/googletest/include)
add_executable(Tests test.cpp ../server/server.cpp ../server/session/session.cpp ../server/room/room.cpp)

target_link_libraries(Tests gtest gmock gtest_main)

