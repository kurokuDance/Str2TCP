cmake_minimum_required(VERSION 3.10)
project(Program1)

find_package(Boost REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

add_executable(Program1 Source.cpp)

target_link_libraries(Program1 ${Boost_LIBRARIES})
