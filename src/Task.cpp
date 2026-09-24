#include "Task.hpp"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <utility>

std::string Task::validateText(std::string input_text)
{
  if (std::all_of(input_text.begin(), input_text.end(), [](unsigned char character)
                  { return std::isspace(character); }))
  {
    throw std::invalid_argument("Task text cannot be empty.");
  }

  return input_text;
}

std::chrono::year_month_day Task::validateDeadline(std::chrono::year_month_day date)
{
  if (!date.ok())
  {
    throw std::invalid_argument("Invalid date.");
  }

  return date;
}

Task::Task(std::string input_text, Priority new_priority)
    : Task(std::move(input_text), {}, new_priority)
{
}

Task::Task(
    std::string input_text,
    std::optional<std::chrono::year_month_day> new_deadline,
    Priority new_priority)
    : text{validateText(std::move(input_text))},
      deadline{new_deadline},
      priority{new_priority}
{
  if (new_deadline)
  {
    deadline = validateDeadline(new_deadline.value());
  }
}

void Task::changeText(std::string new_text)
{
  text = validateText(std::move(new_text));
}

void Task::changeDeadline(std::chrono::year_month_day date)
{
  deadline = validateDeadline(date);
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

std::ostream &operator<<(
    std::ostream &os,
    const Task &task)
{
  os << std::left << std::setw(15) << task.text << " | ";

  if (task.deadline.has_value())
  {
    const auto &date = task.deadline.value();
    std::ostringstream date_format{};

    date_format << "Deadline: "
                << static_cast<unsigned>(date.day())
                << '-' << static_cast<unsigned>(date.month())
                << '-' << static_cast<int>(date.year());

    os << std::left << std::setw(22) << date_format.str() << " | ";
  }
  else
  {
    os << std::left << std::setw(22) << "Deadline: None" << " | ";
  }

  switch (task.priority)
  {
  case Priority::HIGH:
    os << std::setw(20) << "Priority: High" << " | ";
    break;
  case Priority::MEDIUM:
    os << std::setw(20) << "Priority: Medium" << " | ";
    break;
  case Priority::LOW:
    os << std::setw(20) << "Priority: Low" << " | ";
    break;
  }

  if (task.completed)
  {
    os << "Status: Completed";
  }
  else
  {
    os << "Status: Incomplete";
  }

  return os;
}

const std::string &Task::getText() const
{
  return text;
}

const std::optional<std::chrono::year_month_day> &Task::getDeadline() const
{
  return deadline;
}

Priority Task::getPriority() const
{
  return priority;
}

bool Task::isCompleted() const
{
  return completed;
}