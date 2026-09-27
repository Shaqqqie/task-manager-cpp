#pragma once

#include "Task.hpp"

#include <string>

Priority parsePriority(const std::string &priority_string);
std::string priorityToString(Priority priority);