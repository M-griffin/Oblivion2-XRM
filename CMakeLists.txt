# Top-level CMakeLists.txt for the main project
cmake_minimum_required(VERSION 3.25)
project(xrm-server VERSION 1.0 LANGUAGES CXX)

# Needed by Conan Package Manager
enable_language(C)

# CMake Build Type
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif ()


set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Available configurations" FORCE)

# C++17 standard eventually
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WIN32_EXECUTABLE FALSE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable GTEST Unit Tests
#enable_testing()

# Silence specific warnings if needed
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wno-unused-result -Wno-deprecated-declarations")

# Add this line before adding the executable
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--subsystem,console")
endif ()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Define root source directory and main executable source files
file(GLOB MAIN_SOURCES "src/*.cpp" "src/**/*.cpp")
file(GLOB MAIN_HEADERS "src/*.hpp" "src/**/*.hpp" "src/**/*.h")

# Win32 Specific Compiler Flags
if (WIN32)
    add_executable(xrm-server ${MAIN_SOURCES})
    # Force static linking

    target_link_options(xrm-server PRIVATE -static)

    set_property(TARGET xrm-server PROPERTY WIN32_EXECUTABLE False)

    target_link_options(xrm-server PRIVATE
            PRIVATE "-Wl,--subsystem,console"
    )

else ()
    add_executable(xrm-server ${MAIN_SOURCES})
endif ()

# PreProcessors -D Statements
target_compile_definitions(xrm-server PRIVATE -DWITHOUT_SDL)

# target_link_libraries(xrm-server PRIVATE pthreads)

# Default Win Sockets Libs for SDL2_NET
target_link_libraries(xrm-server PRIVATE iphlpapi)
target_link_libraries(xrm-server PRIVATE wsock32)
target_link_libraries(xrm-server PRIVATE ws2_32)

# Find SQLite3 (Conan)
find_package(Sqlite3 REQUIRED)
target_link_libraries(xrm-server PRIVATE SQLite::SQLite3)

# Encryption Algos (Conan)
find_package(OpenSSL REQUIRED)
target_link_libraries(xrm-server PRIVATE OpenSSL::SSL)

# Menu and Data Files (Conan)
find_package(yaml-cpp REQUIRED)
target_link_libraries(xrm-server PRIVATE yaml-cpp::yaml-cpp)

find_package(utf8cpp REQUIRED)
target_link_libraries(xrm-server PRIVATE utf8cpp)

# TESTING - WIP!  This is for TESTS Subfolder, not main project.
#find_package(GTest REQUIRED)
#target_link_libraries(xrm-server PRIVATE gtest::gtest)


# SQLLite Include directories
target_include_directories(xrm-server
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${SQLITE3_INCLUDE_DIRS}
)

# Configuration-specific build logic
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    # Set include paths from SQLitewrap (assumes it exports headers or uses install)
    include_directories(${PROJECT_SOURCE_DIR}/sqlitewrap/cmake-build-debug/include)
    include_directories(${PROJECT_SOURCE_DIR}/sqlitewrap/include)

    # Use debug build from sqlitewrap
    target_compile_definitions(xrm-server PRIVATE DEBUG)
    if (WIN32)
        target_link_libraries(xrm-server PRIVATE "${PROJECT_SOURCE_DIR}/sqlitewrap/cmake-build-debug/lib/libSqliteWrapped.a")
    else ()
        target_link_libraries(xrm-server PRIVATE "${PROJECT_SOURCE_DIR}/sqlitewrap/cmake-build-debug/lib/libSqliteWrapped.a")
    endif ()
elseif (CMAKE_BUILD_TYPE MATCHES "Release")
    # Set include paths from SQLitewrap (assumes it exports headers or uses install)
    include_directories(${PROJECT_SOURCE_DIR}/sqlitewrap/cmake-build-release/include)
    include_directories(${PROJECT_SOURCE_DIR}/sqlitewrap/include)

    # Use release build from sqlitewrap
    target_compile_definitions(xrm-server PRIVATE NDEBUG)
    if (WIN32)
        target_link_libraries(xrm-server PRIVATE "${PROJECT_SOURCE_DIR}/sqlitewrap/cmake-build-release/lib/libSqliteWrapped.a")
    else ()
        target_link_libraries(xrm-server PRIVATE "${PROJECT_SOURCE_DIR}/sqlitewrap/cmake-build-release/lib/libSqliteWrppped.a")
    endif ()
endif ()
