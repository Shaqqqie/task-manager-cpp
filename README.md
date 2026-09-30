# Task Manager C++

A command-line task management application built with modern C++.

This project was created to practice designing a larger C++ application with separation of responsibilities, persistent storage, automated testing, and a CMake-based build system.

## Features

- Add and remove tasks
- View all tasks
- Edit existing tasks
- Mark tasks as complete or incomplete
- Assign task priorities
- Set optional deadlines
- Persistent task storage using JSON
- Input validation
- Automated tests

## Technologies

- **C++20**
- **CMake**
- **Catch2 3.8.1** for automated testing
- **nlohmann/json 3.12.0** for JSON serialization
- **GitHub Actions** for continuous integration

## Project Structure

```text id="e0i1d6"
task-manager-cpp/
├── data/
│   └── tasks.json
├── include/
│   ├── DateUtils.hpp
│   ├── PriorityUtils.hpp
│   ├── Task.hpp
│   ├── TaskManager.hpp
│   └── TaskStorage.hpp
├── src/
│   ├── DateUtils.cpp
│   ├── PriorityUtils.cpp
│   ├── Task.cpp
│   ├── TaskManager.cpp
│   ├── TaskStorage.cpp
│   └── main.cpp
├── tests/
│   ├── TaskManagerTests.cpp
│   ├── TaskStorageTests.cpp
│   └── TaskTests.cpp
├── CMakeLists.txt
└── README.md
```

The application is divided into separate components:

- `Task` represents an individual task and its state.
- `TaskManager` manages the collection of tasks and task operations.
- `TaskStorage` handles persistent JSON storage.
- `DateUtils` contains date-related functionality.
- `PriorityUtils` contains priority-related functionality.
- `main.cpp` provides the command-line interface.

## Building the Project

### Requirements

- A C++20-compatible compiler
- CMake 3.20 or newer
- Git

Dependencies are downloaded automatically by CMake using `FetchContent`.

### Build

```bash id="8do6kt"
git clone https://github.com/Shaqqqie/task-manager-cpp.git
cd task-manager-cpp

cmake -S . -B build
cmake --build build
```

## Running

After building the project:

```bash id="bfhm83"
./build/task_manager
```

The application provides an interactive command-line interface for creating, viewing, editing, removing, and completing tasks.

Task data is stored in JSON so that tasks can persist between program runs.

## Running the Tests

The project uses Catch2 for automated testing.

```bash id="ou7e41"
ctest --test-dir build --output-on-failure
```

Tests cover the core task model, task-management operations, and persistent storage.

## What I Practiced

This project helped me gain experience with:

- Object-oriented design in C++
- Separating responsibilities across classes
- `std::vector`
- `std::optional`
- `std::chrono`
- Enumerations
- File persistence
- JSON serialization and deserialization
- Input validation
- Exception handling
- Automated testing with Catch2
- Dependency management with CMake `FetchContent`
- Compiler warnings and code quality
- Continuous integration with GitHub Actions
- Git and GitHub workflow

## Future Improvements

Possible future improvements include:

- Filtering tasks
- Sorting tasks
- Searching tasks by text
- Task categories or tags
- Improved command-line interface
- Additional test coverage

## Purpose

This project is part of my C++ learning journey and focuses on moving beyond small programming exercises toward structured, testable applications.

Rather than keeping all functionality in a single source file, the project separates task management, persistence, utility functionality, user interaction, and testing to practice maintainable software design.