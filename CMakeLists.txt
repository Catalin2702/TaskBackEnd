cmake_minimum_required(VERSION 3.30)
project(BackEnd)

set(CMAKE_CXX_STANDARD 17)

# Aggiungi l'eseguibile principale
add_executable(BackEnd src/main.cpp
        src/params.hpp)

# Aggiungi la libreria orm
add_subdirectory(lib/orm)
target_link_libraries(BackEnd orm)


if(APPLE)
    set_target_properties(BackEnd PROPERTIES
            BUILD_WITH_INSTALL_RPATH TRUE
            INSTALL_RPATH "@loader_path;@loader_path/../lib"
    )
endif()
