#include "Task.hpp"

#include <utility>

Task::Task(
    std::string input_text,
    std::optional<std::chrono::year_month_day> new_deadline,
    Priority new_priority)
    : text{std::move(input_text)},
      deadline{new_deadline},
      priority{new_priority}
{
}