#include "TaskStorage.hpp"

#include <fstream>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <sstream>
#include <stdexcept>
#include <string>

namespace
{
    std::string priorityToString(Priority priority)
    {
        switch (priority)
        {
        case Priority::HIGH:
            return "HIGH";

        case Priority::MEDIUM:
            return "MEDIUM";

        case Priority::LOW:
            return "LOW";
        }

        throw std::invalid_argument("Invalid priority.");
    }

    std::string dateToString(std::chrono::year_month_day date)
    {
        std::ostringstream output;

        output << static_cast<int>(date.year())
               << '-'
               << std::setfill('0') << std::setw(2)
               << static_cast<unsigned>(date.month())
               << '-'
               << std::setw(2)
               << static_cast<unsigned>(date.day());

        return output.str();
    }

    Priority stringToPriority(const std::string &priority)
    {
        if (priority == "HIGH")
        {
            return Priority::HIGH;
        }
        else if (priority == "MEDIUM")
        {
            return Priority::MEDIUM;
        }
        else if (priority == "LOW")
        {
            return Priority::LOW;
        }

        throw std::invalid_argument("Invalid priority.");
    }

    std::chrono::year_month_day stringToDate(const std::string &date_string)
    {
        std::istringstream input{date_string};

        int year{};
        unsigned month{};
        unsigned day{};
        char separator1{};
        char separator2{};

        input >> year >> separator1 >> month >> separator2 >> day;

        if (!input || separator1 != '-' || separator2 != '-')
        {
            throw std::invalid_argument("Invalid date format.");
        }

        char extra{};
        if (input >> extra)
        {
            throw std::invalid_argument("Invalid date format");
        }

        std::chrono::year_month_day date{
            std::chrono::year{year},
            std::chrono::month{month},
            std::chrono::day{day}};

        if (!date.ok())
        {
            throw std::invalid_argument("Invalid date.");
        }

        return date;
    }
}

void TaskStorage::save(const std::vector<Task> &tasks, const std::filesystem::path &path)
{
    nlohmann::json json_tasks = nlohmann::json::array();

    for (const Task &task : tasks)
    {
        nlohmann::json json_task;

        json_task["text"] = task.getText();
        json_task["priority"] = priorityToString(task.getPriority());
        json_task["completed"] = task.isCompleted();

        if (task.getDeadline().has_value())
        {
            json_task["deadline"] = dateToString(task.getDeadline().value());
        }
        else
        {
            json_task["deadline"] = nullptr;
        }

        json_tasks.push_back(json_task);
    }

    std::ofstream file{path};

    if (!file)
    {
        throw std::runtime_error("Failed to open file for writing.");
    }

    file << std::setw(4) << json_tasks;

    if (!file)
    {
        throw std::runtime_error("Failed to write tasks to file.");
    }
}

std::vector<Task> TaskStorage::load(const std::filesystem::path &path)
{
    std::ifstream file{path};

    if (!file)
    {
        throw std::runtime_error("Failed to open file for reading");
    }

    nlohmann::json json_tasks;
    try
    {
        file >> json_tasks;

        std::vector<Task> tasks{};

        for (const nlohmann::json &json_task : json_tasks)
        {
            std::string text{json_task.at("text").get<std::string>()};
            bool completed{json_task.at("completed").get<bool>()};

            std::string priority_string{json_task.at("priority").get<std::string>()};
            Priority priority{stringToPriority(priority_string)};

            std::optional<std::chrono::year_month_day> deadline{};

            if (!json_task.at("deadline").is_null())
            {
                std::string deadline_string{json_task.at("deadline").get<std::string>()};

                deadline = stringToDate(deadline_string);
            }

            Task task{std::move(text), deadline, priority};

            if (completed)
            {
                task.markComplete();
            }

            tasks.push_back(std::move(task));
        }

        return tasks;
    }

    catch (const nlohmann::json::exception &error)
    {
        throw std::runtime_error(
            std::string{"JSON error: "} + error.what());
    }
}