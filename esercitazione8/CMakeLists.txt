cmake_minimum_required(VERSION 3.20)
project(esercitazione8) 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#attivo warning a compilatore
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla -Wno-sign-compare")

#i file header sono inclusi nel main.cpp
SET(SOURCES main.cpp unidirected_edge.hpp unidirected_graph.hpp)

add_executable(test_grafi ${SOURCES})
