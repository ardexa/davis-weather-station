cmake_minimum_required(VERSION 2.6)

project(read_davis)

set(CMAKE_CXX_FLAGS "-Wall -fexceptions -std=c++11")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES /usr/local/lib ${CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES})

set(davis_SRCS
    arguments.cpp
    arguments.hpp
    configs.hpp
    main.cpp
    utils.cpp
    utils.hpp
)

add_executable(read_davis
    ${davis_SRCS}
)

target_link_libraries(read_davis
    udev
)
