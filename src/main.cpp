#include "Task.hpp"

#include <chrono>
#include <iostream>
#include <string>

int main()
{
   

    try
    {
        Task task{""};

        std::cout << "Task created.\n";
    }
    catch(const std::invalid_argument& error)
    {
        std::cout << error.what() << '\n';
    }

    return 0;
}