cmake_minimum_required(VERSION 3.10)
project(ej1) # Nombre del proyecto
file(GLOB SOURCE src/*.cpp example/main.cpp)
add_executable(run ${SOURCE})