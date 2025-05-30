cmake_minimum_required(VERSION 3.10)

project(Setker LANGUAGES CXX)

# Estándar de C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Incluir rutas de encabezados
# Usar target_include_directories para el ejecutable
file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src/def
    ${CMAKE_SOURCE_DIR}/src/commands
)

# Configurar carpeta de salida para bins:
set_target_properties(${PROJECT_NAME} PROPERTIES
  # Para generadores single-config (Makefile, Ninja)
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>
  # Para generadores multi-config (Visual Studio)
  RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_SOURCE_DIR}/bin/Debug
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release
)