cmake_minimum_required(VERSION 3.20)
project(VSProjectPort C)

set(CMAKE_C_STANDARD 99)

# Buscar automáticamente todos los archivos .c y .h en el directorio actual
file(GLOB SOURCES "*.c" "*.h")

# Crear el ejecutable con todos los archivos encontrados
add_executable(VSProjectPort ${SOURCES})

target_link_libraries(VSProjectPort ws2_32)
