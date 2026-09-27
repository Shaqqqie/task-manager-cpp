#include "DateUtils.hpp"
#include "PriorityUtils.hpp"
#include "Task.hpp"
#include "TaskManager.hpp"
#include "TaskStorage.hpp"

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <limits>
#include <string>

void addTask(TaskManager &manager);
void viewTasks(const TaskManager &manager);
bool removeTask(TaskManager &manager);

int main()
{
    const std::filesystem::path data_path{"data/tasks.json"};

    std::filesystem::create_directories(data_path.parent_path());

    TaskManager manager;

    if (std::filesystem::exists(data_path))
    {
        auto tasks = TaskStorage::load(data_path);
        manager.setTasks(std::move(tasks));
    }

    int menu_choice{};
    while (true)
    {
        std::cout << "\n\n=====Task Manager=====\n\n"
                  << "1. Add task\n"
                  << "2. View tasks\n"
                  << "3. Edit task\n"
                  << "4. Remove task\n"
                  << "5. Mark task complete/incomplete\n"
                  << "6. Filter tasks\n"
                  << "7. Sort tasks\n"
                  << "8. Exit\n"
                  << "Choose option: ";

        if (!(std::cin >> menu_choice))
        {
            std::cout << "Not valid input.\n";

            std::cin.clear();
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n');

            continue;
        }

        switch (menu_choice)
        {
        case 1:
            addTask(manager);
            TaskStorage::save(manager.getTasks(), data_path);
            break;
        case 2:
        {
            if (manager.getTasks().empty())
            {
                std::cout << "\nNo tasks to display.\n";
                break;
            }
            viewTasks(manager);
            break;
        }
        case 3:
            // Edit task
            break;
        case 4:
            if (removeTask(manager))
            {
                TaskStorage::save(manager.getTasks(), data_path);
            }
            break;
        case 5:
            // Mark task complete/incomplete
            break;
        case 6:
            // Filter tasks
            break;
        case 7:
            // sort tasks
            break;
        case 8:
            TaskStorage::save(manager.getTasks(), data_path);
            std::cout << "Tasks saved. Goodbye!\n";
            return 0;
        default:
            std::cout << "\nInvalid option.\n";
            break;
        }
    }

    return 0;
}

void addTask(TaskManager &manager)
{
    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n');

    std::string text{};
    std::cout << "Enter text: ";
    std::getline(std::cin, text);

    std::optional<std::chrono::year_month_day> deadline{};
    char input_add_deadline{};
    std::cout << "Add deadline?(Y/N)";

    if (std::cin >> input_add_deadline && (input_add_deadline == 'Y' || input_add_deadline == 'y'))
    {
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n');

        std::string date{};
        std::cout << "Enter date(YYYY-MM-DD): ";
        std::cin >> date;

        deadline = parseDate(date);
    }

    Priority priority{Priority::LOW};

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n');

    std::cout << "Priority (HIGH, MEDIUM, LOW) [default: low]: ";

    std::string priority_string{};
    std::getline(std::cin, priority_string);

    if (!priority_string.empty())
    {
        std::transform(
            priority_string.begin(),
            priority_string.end(),
            priority_string.begin(),
            [](unsigned char c)
            {
                return static_cast<char>(std::toupper(c));
            });

        priority = parsePriority(priority_string);
    }

    manager.addTask(std::move(text), deadline, priority);

    std::cout << "\nTask successfully added.\n";
}

void viewTasks(const TaskManager &manager)
{
    const auto &tasks = manager.getTasks();

    std::cout << '\n';

    for (std::size_t i{0}; i < tasks.size(); ++i)
    {
        std::cout << i + 1 << ". " << tasks.at(i) << '\n';
    }
}

bool removeTask(TaskManager &manager)
{
    viewTasks(manager);

    const auto &tasks = manager.getTasks();

    if (tasks.empty())
    {
        std::cout << "\nNo tasks to remove.\n";
        return false;
    }

    std::string input{};
    int task_number{};

    while (true)
    {
        std::cout << "Task to remove (Q to cancel): ";
        std::cin >> input;

        if (input == "Q" || input == "q")
        {
            std::cout << "\nCanceled remove task operation.\n";
            return false;
        }

        try
        {
            std::size_t pos{};
            task_number = std::stoi(input, &pos);

            if(pos != input.size())
            {
                std::cout << "\nInvalid task number.\n";
                continue;
            }

            if (task_number > 0 && task_number <= static_cast<int>(tasks.size()))
            {
                break;
            }

            std::cout << "\nInvalid task number.\n";
        }
        catch(const std::exception &)
        {
                std::cout << "\nInvalid task number.\n";
        }
    }

    std::size_t index{static_cast<size_t>(task_number) - 1};

    manager.removeTask(index);

    std::cout << "\nTask removed.\n";
    return true;
}