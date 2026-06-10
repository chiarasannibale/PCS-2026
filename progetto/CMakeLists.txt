
cmake_minimum_required(VERSION 3.20)
project(progetto_finale)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla")

find_package(Eigen3 REQUIRED)

add_executable(progetto_finale main.cpp)

target_link_libraries(progetto_finale Eigen3::Eigen)