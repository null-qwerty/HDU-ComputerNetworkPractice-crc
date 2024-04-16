CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

project(crc)

include_directories(
    include
)

add_executable(crc src/main.cpp src/crc/crc.cpp)