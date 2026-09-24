#include "TaskManager.hpp"

#include <algorithm>
#include <iterator>
#include <utility>

void TaskManager::addTask(std::string text, std::optional<std::chrono::year_month_day> deadline, Priority priority)
{
    tasks.emplace_back(std::move(text), deadline, priority);
}

void TaskManager::addTask(std::string text, Priority priority)
{
    tasks.emplace_back(std::move(text), priority);
}

const std::vector<Task> &TaskManager::getTasks() const
{
    return tasks;
}

bool TaskManager::removeTask(std::size_t index)
{
    if (index >= tasks.size())
    {
        return false;
    }

    tasks.erase(tasks.begin() + index);

    return true;
}

bool TaskManager::editTaskText(std::size_t index, std::string new_text)
{
    if (index >= tasks.size())
    {
        return false;
    }

    Task &task = tasks.at(index);
    task.changeText(std::move(new_text));

    return true;
}

bool TaskManager::editTaskDeadline(std::size_t index, std::optional<std::chrono::year_month_day> new_deadline)
{
    if (index >= tasks.size())
    {
        return false;
    }

    Task &task = tasks.at(index);

    if (new_deadline)
    {
        task.changeDeadline(new_deadline.value());
    }
    else
    {
        task.removeDeadline();
    }

    return true;
}

bool TaskManager::editTaskPriority(std::size_t index, Priority new_priority)
{
    if (index >= tasks.size())
    {
        return false;
    }

    Task &task = tasks.at(index);
    task.changePriority(new_priority);

    return true;
}

bool TaskManager::markTaskComplete(std::size_t index)
{
    if (index >= tasks.size())
    {
        return false;
    }

    Task &task = tasks.at(index);
    task.markComplete();

    return true;
}

bool TaskManager::markTaskIncomplete(std::size_t index)
{
    if (index >= tasks.size())
    {
        return false;
    }

    Task &task = tasks.at(index);
    task.markIncomplete();

    return true;
}

std::vector<Task> TaskManager::filterByCompletion(bool completed) const
{
    std::vector<Task> filtered_by_completion{};

    std::copy_if(tasks.begin(), tasks.end(), std::back_inserter(filtered_by_completion),
                 [completed](const Task &task)
                 { return task.isCompleted() == completed; });

    return filtered_by_completion;
}

std::vector<Task> TaskManager::filterByPriority(Priority priority) const
{
    std::vector<Task> filtered_by_priority{};

    std::copy_if(tasks.begin(), tasks.end(), std::back_inserter(filtered_by_priority),
                 [priority](const Task &task)
                 { return task.getPriority() == priority; });

    return filtered_by_priority;
}

std::vector<Task> TaskManager::filterDueToday(std::chrono::year_month_day today) const
{
    std::vector<Task> filtered_by_today{};

    std::copy_if(tasks.begin(), tasks.end(), std::back_inserter(filtered_by_today),
                 [today](const Task &task)
                 {
                     if (task.getDeadline().has_value())
                     {
                         return task.getDeadline().value() == today;
                     }
                     return false;
                 });

    return filtered_by_today;
}

std::vector<Task> TaskManager::filterOverdue(std::chrono::year_month_day today) const
{
    std::vector<Task> filtered_by_overdue_deadline{};

    std::copy_if(tasks.begin(), tasks.end(), std::back_inserter(filtered_by_overdue_deadline),
                 [today](const Task &task)
                 {
                     if (task.getDeadline().has_value() && !task.isCompleted())
                     {
                         return task.getDeadline().value() < today;
                     }
                     return false;
                 });

    return filtered_by_overdue_deadline;
}

std::vector<Task> TaskManager::filterDueBy(std::chrono::year_month_day date) const
{
    std::vector<Task> filtered_by_date{};

    if (!date.ok())
    {
        throw std::invalid_argument("Invalid date");
    }

    std::copy_if(tasks.begin(), tasks.end(), std::back_inserter(filtered_by_date),
                 [date](const Task &task)
                 {
                     if (task.getDeadline().has_value() && !task.isCompleted())
                     {
                         return task.getDeadline().value() <= date;
                     }
                     return false;
                 });

    return filtered_by_date;
}

std::vector<Task> TaskManager::filterNoDeadline() const
{
    std::vector<Task> filtered_by_no_deadline{};

    std::copy_if(tasks.begin(), tasks.end(), std::back_inserter(filtered_by_no_deadline),
                 [](const Task &task)
                 {
                     return !task.getDeadline().has_value();
                 });

    return filtered_by_no_deadline;
}