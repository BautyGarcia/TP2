cmake_minimum_required(VERSION 3.10)
project(ej1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#declaro que archivos debe compilar
file(GLOB SOURCE src/* example/main.cpp)
add_executable(main ${SOURCE})

#cambio el directorio de salida del ejecutable para que se cree en bin
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#flags de warnings
target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic)

#esto es para que se corra solo al hacer make run
add_custom_target(run
    COMMAND main
    DEPENDS main
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
