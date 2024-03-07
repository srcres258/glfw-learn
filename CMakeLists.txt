cmake_minimum_required(VERSION 3.27)
project(glfw_learn)

set(CMAKE_CXX_STANDARD 23)

link_directories(/usr/lib)
link_libraries(/usr/lib/libglfw.so)

INCLUDE_DIRECTORIES(include)

add_executable(glfw_learn main.cpp
        glad.c
        shader.h
        shader.cpp)
