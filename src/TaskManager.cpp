#include "TaskManager.hpp"

#include <utility>


void TaskManager::addTask(std::string text, std::optional < std::chrono::year_month_day> deadline, Priority priority)
{
    tasks.emplace_back(std::move(text), deadline, priority);
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