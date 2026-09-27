#pragma once

#include <chrono>
#include <string>

std::chrono::year_month_day parseDate(const std::string &date_string);
std::string dateToString(std::chrono::year_month_day date);
