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
    bool editTaskText(std::size_t index, std::string new_text);
    bool editTaskDeadline(std::size_t index, std::optional<std::chrono::year_month_day> new_deadline);
    bool editTaskPriority(std::size_t index, Priority new_priority);
    bool markTaskComplete(std::size_t index);
    bool markTaskIncomplete(std::size_t index);

    const std::vector<Task> &getTasks() const;
};