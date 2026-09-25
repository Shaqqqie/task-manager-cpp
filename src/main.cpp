#include "Task.hpp"
#include "TaskManager.hpp"

#include <chrono>
#include <iostream>
#include <string>

int main()
{
    TaskManager manager;

    std::chrono::year_month_day date{
        std::chrono::year{2026},
        std::chrono::month{9},
        std::chrono::day{22}};

    
    manager.addTask("Study C++", date, Priority::MEDIUM);
    manager.addTask("Buy groceries");
    
    manager.removeTask(0);

    auto &tasks = manager.getTasks();

    for(const auto &task: tasks)
    {
        std::cout << task << '\n';
    }

 /*
    try
    {
        manager.editTaskText(index, new_text)
    }
    catch(const std::invalid_argument &error)
    {
        std::cout << "Unable to edit task: " << error.what() << '\n'
    }
 */

    return 0;
}