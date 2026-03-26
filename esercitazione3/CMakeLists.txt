
project(es3) 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall ")

set (SOURCES esercizio3.cpp)
add_executable(es3 ${SOURCES} )

