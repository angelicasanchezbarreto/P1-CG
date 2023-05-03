cmake_minimum_required(VERSION 3.19)
project(CG2023)

set(CMAKE_CXX_STANDARD 20)

add_executable(CG2023 main.cpp vec3.cpp vec3.h Ray.cpp Ray.h
        Object.cpp Object.h Camera.cpp Camera.h Light.cpp Light.h)