cmake_minimum_required(VERSION 3.27)
project(glfw_learn)

set(CMAKE_CXX_STANDARD 23)

link_directories(/usr/lib)
link_libraries(/usr/lib/libglfw.so)
link_libraries(/usr/lib/libassimp.so)

include_directories(include)

add_executable(glfw_learn main1.cpp
        glad.c
        shader.cpp
        stb_image.cpp
        camera.cpp
        model.cpp)
