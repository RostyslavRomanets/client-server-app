cmake_minimum_required(VERSION 3.27)
project(client_server)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.84.0 REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

add_executable(server server/main.cpp)

add_executable(client client/main.cpp)
