cmake_minimum_required(VERSION 3.30)
project(BackEnd)

set(CMAKE_CXX_STANDARD 17)

# Configura cpp-httplib
include(FetchContent)
FetchContent_Declare(
        httplib
        GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
        GIT_TAG v0.14.1
)
FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_Declare(
        picosha2
        GIT_REPOSITORY https://github.com/okdshin/PicoSHA2.git
        GIT_TAG master
)
FetchContent_MakeAvailable(httplib json picosha2)

# Aggiungi l'eseguibile principale
add_executable(BackEnd src/main.cpp
        src/params.hpp

        include/api/controller/c_user.hpp
        include/api/controller/controller.hpp
        include/api/model/m_user.hpp
        include/api/service/s_user.hpp
        include/api/service/service.hpp
        include/api/server.hpp

        src/api/controller/c_user.cpp
        src/api/controller/controller.cpp
        src/api/model/m_user.cpp
        src/api/service/s_user.cpp
        src/api/service/service.cpp
        src/api/server.cpp
        include/api/controller/controller.tpp
)

# Aggiungi la libreria orm
add_subdirectory(lib/orm)

include_directories(include)

if (APPLE)
    set_target_properties(BackEnd PROPERTIES
            BUILD_WITH_INSTALL_RPATH TRUE
            INSTALL_RPATH "@loader_path;@loader_path/../lib;${CPP_HTTPLIB_ROOT}"
    )
endif ()

# Corretto ordine in target_link_libraries
target_link_libraries(BackEnd
        PRIVATE
        httplib::httplib
        nlohmann_json::nlohmann_json
        orm
        picosha2
)

target_precompile_headers(BackEnd PRIVATE
        <httplib.h>
)