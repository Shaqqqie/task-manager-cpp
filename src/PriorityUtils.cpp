#include "PriorityUtils.hpp"

Priority parsePriority(const std::string &priority_string)
{
    if (priority_string == "HIGH")
    {
        return Priority::HIGH;
    }
    else if (priority_string == "MEDIUM")
    {
        return Priority::MEDIUM;
    }
    else if (priority_string == "LOW")
    {
        return Priority::LOW;
    }

    throw std::invalid_argument("Invalid priority.");
}

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