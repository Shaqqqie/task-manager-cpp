#pragma once

#include "Task.hpp"

#include <filesystem>
#include <vector>

class TaskStorage
{
    public:
    static void save(
        const std::vector<Task> &tasks,
        const std::filesystem::path &path
    );

    static std::vector<Task> load(
        const std::filesystem::path &path
    );
};