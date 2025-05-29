cmake_minimum_required(VERSION 3.10)
project(ej1)

#declaro que archivos debe compilar
file(GLOB SOURCE src/*.cpp example/main.cpp)
add_executable(main ${SOURCE})

#cambio el directorio de salida del ejecutable para que se cree en bin
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#flags de warnings
target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic -Werror -Wconversion -Wsign-conversion -Wshadow -Wnull-dereference -Wfloat-equal -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wformat=2 -Wcast-align -Wstrict-overflow=5 -Wunsafe-loop-optimizations -Wuseless-cast -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wnon-virtual-dtor -Wzero-as-null-pointer-constant -Wmissing-declarations -Weffc++ -Wstack-protector)

#esto es para que se corra solo al hacer make run
add_custom_target(run
    COMMAND main
    DEPENDS main
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
