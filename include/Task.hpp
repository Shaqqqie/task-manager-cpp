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

    static std::string validateText(std::string input_text);
    static std::chrono::year_month_day validateDeadline(std::chrono::year_month_day date);

public:
    Task(std::string input_text, std::optional<std::chrono::year_month_day> new_deadline = {},
         Priority new_priority = Priority::LOW);
    
    void changeText(std::string new_text);
    
    void changeDeadline(std::chrono::year_month_day date);
    void removeDeadline();
    
    void changePriority(Priority new_priority);
    
    void markComplete();
    void markIncomplete();


};