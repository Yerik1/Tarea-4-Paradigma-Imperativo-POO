cmake_minimum_required(VERSION 3.20)
project(Servidor C)

set(CMAKE_C_STANDARD 99)

add_executable(Servidor main.c)

# Enlazar la biblioteca Winsock
target_link_libraries(Servidor ws2_32)

