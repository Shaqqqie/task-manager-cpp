cmake_minimum_required(VERSION 3.20)

project(task_manager VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.1
)

FetchContent_MakeAvailable(Catch2)

enable_testing()

add_library(task_manager_lib
    src/Task.cpp
    src/TaskManager.cpp
)

target_include_directories(task_manager_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

if (MSVC)
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

add_executable(task_manager 
    src/main.cpp
)

target_link_libraries(task_manager PRIVATE
    task_manager_lib
)

if (MSVC)
    target_compile_options(task_manager PRIVATE
        /W4
    )
else()
    target_compile_options(task_manager PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )

add_executable(task_manager_tests
    tests/TaskTests.cpp
    tests/TaskManagerTests.cpp
)

target_link_libraries(task_manager_tests PRIVATE
    task_manager_lib
    Catch2::Catch2WithMain
)

include(Catch)

catch_discover_tests(task_manager_tests)