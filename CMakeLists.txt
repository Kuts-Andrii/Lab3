cmake_minimum_required(VERSION 3.25)
project(Lab_Threads)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Lab_Threads main.cpp)

find_package(Threads REQUIRED)
target_link_libraries(Lab_Threads PRIVATE Threads::Threads)