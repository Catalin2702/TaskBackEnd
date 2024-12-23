# CMakeLists.txt for the orm library

# Add the library
add_library(orm
        include/orm/column.hpp
        include/orm/column.tpp
        include/orm/database.hpp
        include/orm/model.hpp
        include/orm/model.tpp
        include/orm/session.tpp
        include/orm/session.hpp
        include/tools/tools.hpp
        include/tools/tools.tpp

        src/column.cpp
        src/database.cpp
        src/model.cpp
        src/tools.cpp
        src/session.cpp
)

target_include_directories(orm PUBLIC include)

# Set PostgreSQL path
set(PostgreSQL_ROOT "/opt/homebrew/opt/postgresql@14")
set(PQXX_ROOT "/opt/homebrew/opt/libpqxx")

# Find PostgreSQL
find_package(PostgreSQL REQUIRED)

# Trova libpqxx
find_library(PQXX_LIBRARY
        NAMES libpqxx.a pqxx
        PATHS ${PQXX_ROOT}/lib
        REQUIRED
)

# Use the libPQ found by find_package(PostgreSQL)
set(PQ_LIBRARY ${PostgreSQL_LIBRARIES})

# Include directories
target_include_directories(orm PUBLIC
        include
        ${PostgreSQL_INCLUDE_DIRS}
        ${PQXX_ROOT}/include
)

# Link libraries
target_link_libraries(orm
        ${PQXX_LIBRARY}
        ${PQ_LIBRARY}
        nlohmann_json::nlohmann_json
)

# Set rpath
if(APPLE)
    set_target_properties(orm PROPERTIES
            BUILD_WITH_INSTALL_RPATH TRUE
            INSTALL_RPATH "@loader_path;@loader_path/../lib;${PostgreSQL_ROOT}/lib;${PQXX_ROOT}/lib"
    )
endif()