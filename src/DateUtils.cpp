#include "DateUtils.hpp"

#include <stdexcept>
#include <sstream>

std::chrono::year_month_day parseDate(const std::string &date_string)
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
        throw std::invalid_argument("Invalid date format.");
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