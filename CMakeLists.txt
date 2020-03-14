cmake_minimum_required(VERSION 3.15)
project(listPeers)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

find_package(Boost 1.64 REQUIRED)

add_executable(listPeers main.cpp pugixml.cpp dsTest.cpp)
add_custom_command(TARGET listPeers
        POST_BUILD
        COMMAND scp "$<TARGET_FILE:listPeers>" guisrv:)
