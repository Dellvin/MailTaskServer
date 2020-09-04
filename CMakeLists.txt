cmake_minimum_required(VERSION 3.15)
project(EnteringTask_server)

find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -Wall -Wextra -Werror=pedantic")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
enable_testing()


add_executable(EnteringTask_server project/main.cpp project/server/session/session.cpp project/server/room/room.cpp project/server/server.cpp project/server/session/constexp.h project/server/room/constexp.h)

add_subdirectory(project/test)