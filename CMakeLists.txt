cmake_minimum_required(VERSION 3.10)
project(eytzinger)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(eytzinger
    main.cpp
)

target_include_directories(
    eytzinger 
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include
)