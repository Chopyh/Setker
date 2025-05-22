cmake_minimum_required(VERSION 3.10)


# Nombre del proyecto
project(Setker)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Incluir directorios de cabeceras
include_directories(${PROJECT_SOURCE_DIR}/src/def ${PROJECT_SOURCE_DIR}/src/commands)

# Reunir todos los archivos fuente (.cpp) bajo src recursivamente
file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# Crear el ejecutable a partir de SOURCES
add_executable(Setker ${SOURCES})

# Enlazar con la biblioteca estándar de C++ (implícito en la mayoría de sistemas)
target_link_libraries(Setker PRIVATE stdc++)