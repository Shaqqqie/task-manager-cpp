cmake_minimum_required(VERSION 3.20)

project(task_manager VERSION 1.0 LANGUAGES CXX)

# ============================================================
# C++ Configuration
# ============================================================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================
# Dependencies
# ============================================================
include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.1
)

# nlohmann/json
FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(
    Catch2
    nlohmann_json
)

enable_testing()

# ============================================================
# Task Manager Library
# ============================================================

add_library(task_manager_lib
    src/Task.cpp
    src/TaskManager.cpp
)

target_include_directories(task_manager_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

if(MSVC)
    target_compile_options(task_manager_lib PRIVATE
        /W4
    )
else()
    target_compile_options(task_manager_lib PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# ============================================================
# Task Storage Library
# ============================================================

add_library(task_storage_lib
    src/TaskStorage.cpp
)

target_include_directories(task_storage_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(task_storage_lib PUBLIC
    task_manager_lib
    nlohmann_json::nlohmann_json
)

if(MSVC)
    target_compile_options(task_storage_lib PRIVATE
        /W4
    )
else()
    target_compile_options(task_storage_lib PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# ============================================================
# Application
# ============================================================

add_executable(task_manager
    src/main.cpp
)

target_link_libraries(task_manager PRIVATE
    task_manager_lib
    task_storage_lib
)

if(MSVC)
    target_compile_options(task_manager PRIVATE
        /W4
    )
else()
    target_compile_options(task_manager PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# ============================================================
# Tests
# ============================================================

add_executable(task_manager_tests
    tests/TaskTests.cpp
    tests/TaskManagerTests.cpp
    tests/TaskStorageTests.cpp
)

target_link_libraries(task_manager_tests PRIVATE
    task_manager_lib
    task_storage_lib
    Catch2::Catch2WithMain
)

include(Catch)

catch_discover_tests(task_manager_tests)