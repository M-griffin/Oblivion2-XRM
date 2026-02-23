# ──────────────────────────────────────────────────────────────────────
# tests/CMakeLists.txt – build the unit‑test executable with GoogleTest
# ──────────────────────────────────────────────────────────────────────

# Find the GoogleTest package that Conan has installed
find_package(GTest REQUIRED)

# Import the CMake module that defines gtest_discover_tests()
include(GoogleTest)

# Find all *.cpp test files in this directory (and sub‑folders)
file(GLOB_RECURSE TEST_SOURCES "*.cpp")

# Build the test executable
add_executable(unit_tests ${TEST_SOURCES})

# Link Main Program as Library into Unit tests.
target_link_libraries(unit_tests PRIVATE xrm-lib)   # <-- link core library

# Link against the main test library provided by GoogleTest
target_include_directories(unit_tests PRIVATE "${PROJECT_SOURCE_DIR}/src/include"
        PRIVATE "${PROJECT_SOURCE_DIR}/src"
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
        PRIVATE ${SQLITE3_INCLUDE_DIRS})

target_link_libraries(unit_tests PRIVATE GTest::gtest_main)

find_package(Sqlite3 REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(utf8cpp REQUIRED)

target_compile_definitions(unit_tests PRIVATE -DWITHOUT_SDL)
target_link_libraries(unit_tests
        PRIVATE iphlpapi wsock32 ws2_32
        PRIVATE SQLite::SQLite3
        PRIVATE OpenSSL::SSL
        PRIVATE yaml-cpp::yaml-cpp
        PRIVATE utf8cpp)
target_link_options(unit_tests PRIVATE -static)

#message(STATUS "UT PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")
#message(STATUS "UT CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")

# Configuration-specific build logic
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    # Set include paths from SQLitewrap (assumes it exports headers or uses install)
    include_directories(${PROJECT_SOURCE_DIR}/sqlitewrap/cmake-build-debug/include)
    include_directories(${PROJECT_SOURCE_DIR}/sqlitewrap/include)

    # Use debug build from sqlitewrap
    target_compile_definitions(unit_tests PRIVATE DEBUG)
    if (WIN32)
        target_link_libraries(unit_tests PRIVATE "${PROJECT_SOURCE_DIR}/sqlitewrap/cmake-build-debug/lib/libSqliteWrapped.a")
    else ()
        target_link_libraries(unit_tests PRIVATE "${PROJECT_SOURCE_DIR}/sqlitewrap/cmake-build-debug/lib/libSqliteWrapped.a")
    endif ()
elseif (CMAKE_BUILD_TYPE MATCHES "Release")
    # Set include paths from SQLitewrap (assumes it exports headers or uses install)
    include_directories(${PROJECT_SOURCE_DIR}/sqlitewrap/cmake-build-release/include)
    include_directories(${PROJECT_SOURCE_DIR}/sqlitewrap/include)

    # Use release build from sqlitewrap
    target_compile_definitions(unit_tests PRIVATE NDEBUG)
    if (WIN32)
        target_link_libraries(unit_tests PRIVATE "${PROJECT_SOURCE_DIR}/sqlitewrap/cmake-build-release/lib/libSqliteWrapped.a")
    else ()
        target_link_libraries(unit_tests PRIVATE "${PROJECT_SOURCE_DIR}/sqlitewrap/cmake-build-release/lib/libSqliteWrppped.a")
    endif ()
endif ()

# Tell CTest to automatically discover all TEST_* / TEST_F() functions
# inside the executable
gtest_discover_tests(unit_tests)

