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
    void addTask(std::string text, Priority priority);
    bool removeTask(std::size_t index);

    const std::vector<Task> &getTasks() const;

    bool editTaskText(std::size_t index, std::string new_text);
    bool editTaskDeadline(std::size_t index, std::optional<std::chrono::year_month_day> new_deadline);
    bool editTaskPriority(std::size_t index, Priority new_priority);

    bool markTaskComplete(std::size_t index);
    bool markTaskIncomplete(std::size_t index);

    std::vector<Task> filterByCompletion(bool completed) const;
    std::vector<Task> filterByPriority(Priority priority) const;
    std::vector<Task> filterDueToday(std::chrono::year_month_day today) const;
    std::vector<Task> filterOverdue(std::chrono::year_month_day date) const;
    std::vector<Task> filterDueBy(std::chrono::year_month_day date) const;
    std::vector<Task> filterNoDeadline() const;

    std::vector<Task> sortByDeadline() const;
    std::vector<Task> sortByPriority() const;

    void setTasks(std::vector<Task> new_tasks);
};