# Top-level CMakeLists.txt for the main project
cmake_minimum_required(VERSION 3.15)
project(SqliteWrapped VERSION 1.0 LANGUAGES CXX)

# Needed by Conan Package Manager
enable_language(C)

# CMake Build Type
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif ()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Available configurations" FORCE)

# C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Install directories
set(INSTALL_INCLUDE_DIR include)
set(INSTALL_LIB_DIR lib)

# Find SQLite3
find_package(Sqlite3 REQUIRED)

# Collect source files
file(GLOB LIB_SOURCES "libsrc/*.cpp")

# Define header files to concatenate
set(HEADER_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/IError.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/StderrLog.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/SysLogs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Database.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Query.h
)

# List of input header files
set(INPUT_HEADER_FILES
        Database.h
        Query.h
        IError.h
        StderrLog.h
        SysLogs.h
)

# Validate all required headers exist
foreach (header IN LISTS HEADER_SOURCES)
    if (NOT EXISTS ${header})
        message(FATAL_ERROR "Missing header: ${header}")
    endif ()
endforeach ()

# Set target for output
set(TARGET_HEADER ${CMAKE_BINARY_DIR}/include/libSqliteWrapped.h)


file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include")

# Platform-specific command for header concatenation
if (WIN32)
    add_custom_command(
            OUTPUT ${TARGET_HEADER}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include
            COMMAND cmd.exe /c type Database.h Query.h IError.h StderrLog.h SysLogs.h > ${TARGET_HEADER}
            DEPENDS ${HEADER_SOURCES}
            COMMENT "Concatenating headers for libSqliteWrapped.h (Windows CMD)"
    )
else ()
    add_custom_command(
            OUTPUT ${TARGET_HEADER}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include
            COMMAND cat Database.h Query.h IError.h StderrLog.h SysLogs.h > ${TARGET_HEADER}
            DEPENDS ${HEADER_SOURCES}
            COMMENT "Concatenating headers for libSqliteWrapped.h (*Nix CAT)"
    )
endif ()

# Create a custom target for the generated header file
add_custom_target(GenerateHeader ALL
        DEPENDS ${TARGET_HEADER}
)

# SQLite3 wrapper library
add_library(SqliteWrapped STATIC ${LIB_SOURCES})

# IS THIS WIN32 ONLY?!?!?
target_compile_options(SqliteWrapped PRIVATE -mconsole -m64)
target_link_options(SqliteWrapped PRIVATE -mconsole -m64)

# Include directories
target_include_directories(SqliteWrapped
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${SQLITE3_INCLUDE_DIRS}
)

# Link SQLite3
target_link_libraries(SqliteWrapped PRIVATE SQLite::SQLite3)

# Ensure headers are generated before building the library
add_dependencies(SqliteWrapped GenerateHeader)

# Install the library and header
install(TARGETS SqliteWrapped
        ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
        LIBRARY DESTINATION ${INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${INSTALL_LIB_DIR}
)

install(FILES ${TARGET_HEADER}
        DESTINATION ${INSTALL_INCLUDE_DIR}
)

# Optional: Add install target for public headers individually, if needed
# install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*.h")
