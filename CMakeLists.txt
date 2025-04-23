cmake_minimum_required(VERSION 3.10)


# Nombre del proyecto
project(Setker)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Añadir los directorios de los encabezados
include_directories(src/def src/commands)

# Añadir los archivos fuente
set(SOURCES
    src/main.cpp
    src/commands/Tokenizer.cpp
    src/def/Keywords.cpp
    src/def/Tokens.cpp
)

# Crear el ejecutable
add_executable(Setker ${SOURCES})

# Enlazar con la biblioteca estándar de C++
target_link_libraries(Setker stdc++)