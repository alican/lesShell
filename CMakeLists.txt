cmake_minimum_required(VERSION 3.1)
project(lesShell)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include(cmake/FindReadline.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lreadline")

set(SOURCE_FILES main.cpp)
add_executable(lesShell ${SOURCE_FILES} JobController.cpp JobController.h Job.cpp Job.h)


