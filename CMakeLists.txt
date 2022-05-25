cmake_minimum_required(VERSION 3.1)

project(RT_sashka)

set(RT_sashka_SRC main.cpp vec3.cpp)
set(RT_sashka_HDR Factory.h main.h Shapes.h vec3.h)

add_executable(${PROJECT_NAME} ${RT_sashka_SRC} ${RT_sashka_HDR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 /std:c++14")