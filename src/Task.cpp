#include "Task.hpp"

#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <utility>

std::string Task::validateText(std::string input_text)
{
  if (std::all_of(input_text.begin(), input_text.end(), [](unsigned char character){return std::isspace(character);}))
  {
    throw std::invalid_argument("Task text cannot be empty.");
  }

  return input_text;
}

Task::Task(
    std::string input_text,
    std::optional<std::chrono::year_month_day> new_deadline,
    Priority new_priority)
    : text{validateText(std::move(input_text))},
      deadline{new_deadline},
      priority{new_priority}
{
  if (new_deadline.has_value() && !new_deadline.value().ok())
  {
    throw std::invalid_argument("Invalid date.");
  }
}

void Task::changeText(std::string new_text)
{
  text = validateText(std::move(new_text));
}

void Task::changeDeadline(std::chrono::year_month_day date)
{
  if (!date.ok())
  {
    throw std::invalid_argument("Invalid date.");
  }

  deadline = date;
}

void Task::removeDeadline()
{
  deadline.reset();
}

void Task::changePriority(Priority new_priority)
{
  priority = new_priority;
}

void Task::markComplete()
{
  completed = true;
}

void Task::markIncomplete()
{
  completed = false;
}