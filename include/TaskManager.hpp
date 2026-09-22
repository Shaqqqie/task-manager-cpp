#pragma once

#include "Task.hpp"

#include <cstddef>
#include <vector>

class TaskManager
{
private:
    std::vector<Task> tasks{};

public:
    void addTask(std::string text, std::optional<std::chrono::year_month_day> deadline = {}, Priority priority = Priority::LOW);
    bool removeTask(std::size_t index);

    const std::vector<Task> &getTasks() const;
    
};