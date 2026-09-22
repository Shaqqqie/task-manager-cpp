#pragma once

#include "Task.hpp"

#include <vector>

class TaskManager
{
private:
    std::vector<Task> tasks{};

public:
    void addTask(std::string text, std::optional<std::chrono::year_month_day> deadline = {}, Priority priority = Priority::LOW);
    const std::vector<Task> &getTasks() const;
    void viewTasks();
};