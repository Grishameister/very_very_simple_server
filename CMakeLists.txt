cmake_minimum_required(VERSION 3.12)
project(highload_server)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Werror -Wpedantic -std=c++1z -pthread")
set(LIBEVENT_NAME "event")
set(LIBEVENT_LIB_DIR "/usr/local/lib")
set(LIBEVENT_INCLUDE_DIR "/usr/local/include")

link_directories("${LIBEVENT_LIB_DIR}/")

include_directories("${LIBEVENT_INCLUDE_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(highload_server main.cpp src/Server.cpp src/ServerSocket.cpp src/Worker.cpp)

target_link_libraries(highload_server ${LIBEVENT_NAME})