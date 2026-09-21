#pragma once

#include <chrono>
#include <optional>
#include <string>

enum class Priority
{
    HIGH,
    MEDIUM,
    LOW
};

class Task
{
private:
    std::string text{};
    std::optional<std::chrono::year_month_day> deadline{};
    bool completed{false};
    Priority priority;

public:
    Task(std::string input_text, std::optional<std::chrono::year_month_day> new_deadline = {},
         Priority new_priority = Priority::LOW);
};