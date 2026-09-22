#include "Task.hpp"

#include <chrono>
#include <iostream>
#include <string>

int main()
{
   std::chrono::year_month_day date{
    std::chrono::year{2026},
    std::chrono::month{9},
    std::chrono::day{22}
   };

    Task task1{"Study C++", date, Priority::MEDIUM};
    Task task2{"Buy groceries"};
    

    std::cout << task1 << '\n';
    std::cout << task2 << '\n';

    return 0;
}