#include "Task.hpp"

#include <catch2/catch_test_macros.hpp>

#include <stdexcept>

TEST_CASE("Task is created with correct default values")
{
    Task task{"Study C++"};

    REQUIRE(task.getText() == "Study C++");
    REQUIRE_FALSE(task.getDeadline().has_value());
    REQUIRE(task.getPriority() == Priority::LOW);
    REQUIRE_FALSE(task.isCompleted());
}

TEST_CASE("Task rejects invalid text")
{
    // Empty string
    REQUIRE_THROWS_AS(
        Task{""},
        std::invalid_argument);

    // Whitespace-only string
    REQUIRE_THROWS_AS(
        Task{"       "},
        std::invalid_argument);
}

TEST_CASE("Task rejects invalid deadline")
{
    std::chrono::year_month_day invalid_date{
        std::chrono::year{2026},
        std::chrono::month{2},
        std::chrono::day{31}};

    REQUIRE_THROWS_AS(
        (Task{"Study C++", invalid_date}),
        std::invalid_argument);
}

TEST_CASE("Task can be modified")
{
    Task task{"Study C++"};

    task.changeText("Study DSA");
    REQUIRE(task.getText() == "Study DSA");

    task.changePriority(Priority::HIGH);
    REQUIRE(task.getPriority() == Priority::HIGH);

    task.markComplete();
    REQUIRE(task.isCompleted());

    task.markIncomplete();
    REQUIRE_FALSE(task.isCompleted());
}

TEST_CASE("Task deadline can be changed and removed")
{
    Task task{"Study C++"};

    REQUIRE_FALSE(task.getDeadline().has_value());

    std::chrono::year_month_day date{
        std::chrono::year{2026},
        std::chrono::month{9},
        std::chrono::day{22}};
    
    task.changeDeadline(date);
    REQUIRE(task.getDeadline().has_value());
    REQUIRE(task.getDeadline().value() == date);

    task.removeDeadline();
    REQUIRE_FALSE(task.getDeadline().has_value());
}