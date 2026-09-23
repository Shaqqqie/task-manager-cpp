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