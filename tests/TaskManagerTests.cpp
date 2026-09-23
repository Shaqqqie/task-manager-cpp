#include "TaskManager.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("TaskManager starts empty and can add tasks")
{
    TaskManager manager;

    REQUIRE(manager.getTasks().empty());

    manager.addTask("Study C++");
    REQUIRE(manager.getTasks().size() == 1);

    const auto &task = manager.getTasks().at(0);

    REQUIRE(task.getText() == "Study C++");
    REQUIRE_FALSE(task.getDeadline().has_value());
    REQUIRE(task.getPriority() == Priority::LOW);
    REQUIRE_FALSE(task.isCompleted());
}

TEST_CASE("removeTask removes the correct element from tasks")
{
    TaskManager manager;

    manager.addTask("Study C++");
    manager.addTask("Buy groceries");
    manager.addTask("Pay bills");

    REQUIRE(manager.removeTask(1));
    
    const auto &tasks = manager.getTasks();

    REQUIRE(tasks.size() == 2);
    REQUIRE(tasks.at(0).getText() == "Study C++");
    REQUIRE(tasks.at(1).getText() == "Pay bills");
}

TEST_CASE("removeTask rejects invalid index")
{
    TaskManager manager;

    manager.addTask("Study C++");
    manager.addTask("Buy groceries");
    manager.addTask("Pay bills");

    REQUIRE_FALSE(manager.removeTask(500));
    REQUIRE(manager.getTasks().size() == 3);
}

TEST_CASE("removeTask rejects index when manager is empty")
{
    TaskManager manager;

    REQUIRE_FALSE(manager.removeTask(0));
    REQUIRE(manager.getTasks().empty());
}

TEST_CASE("editTaskText edits Text for a valid index")
{
    TaskManager manager;

    manager.addTask("Study C++");
    manager.addTask("Buy groceries");
    manager.addTask("Pay bills");

    REQUIRE(manager.editTaskText(1, "Eat Lunch"));
    REQUIRE(manager.getTasks().at(1).getText() == "Eat Lunch");
}

TEST_CASE("editTaskText rejects invalid index and throws exception for invalid text")
{
    TaskManager manager;

    manager.addTask("Study C++");
    manager.addTask("Buy groceries");
    manager.addTask("Pay bills");

    REQUIRE_FALSE(manager.editTaskText(500, "Eat Lunch"));
    REQUIRE_THROWS_AS(manager.editTaskText(1, "     "), std::invalid_argument);
    
}

TEST_CASE("editTaskDeadline edits deadline correctly")
{
    std::chrono::year_month_day date{
        std::chrono::year{2026},
        std::chrono::month{9},
        std::chrono::day{23}
    };

    TaskManager manager;
    manager.addTask("Study C++");
    manager.addTask("Buy groceries");
    manager.addTask("Pay bills");

    const auto &deadline = manager.getTasks().at(1).getDeadline();
    REQUIRE(manager.editTaskDeadline(1, date));
    REQUIRE(deadline.has_value());
    REQUIRE(deadline == date);
}

TEST_CASE("editTaskDeadline removes deadline for no new_deadline input")
{
    std::chrono::year_month_day date{
        std::chrono::year{2026},
        std::chrono::month{9},
        std::chrono::day{23}
    };
    
    TaskManager manager;
    manager.addTask("Study C++");
    manager.addTask("Buy groceries", date);
    manager.addTask("Pay bills");

    REQUIRE(manager.editTaskDeadline(1, {}));
    REQUIRE_FALSE(manager.getTasks().at(1).getDeadline().has_value());
}

TEST_CASE("editTaskDeadline rejects for invalid index")
{
    TaskManager manager;

    manager.addTask("Study C++");
    manager.addTask("Buy groceries");
    manager.addTask("Pay bills");

    REQUIRE_FALSE(manager.editTaskDeadline(500, {}));
}

TEST_CASE("editTaskDeadline throws exception for invalid date")
{
    TaskManager manager;

    manager.addTask("Study C++");

    std::chrono::year_month_day date{
        std::chrono::year{2026},
        std::chrono::month{2},
        std::chrono::day{31}
    };
    REQUIRE_THROWS_AS(manager.editTaskDeadline(0, date ), std::invalid_argument);
}

TEST_CASE("editTaskPriority edits priority for a valid index")
{
    TaskManager manager;

    manager.addTask("Study C++");
    manager.addTask("Buy groceries");
    manager.addTask("Pay bills");

    REQUIRE(manager.editTaskPriority(2, Priority::HIGH));
    REQUIRE(manager.getTasks().at(2).getPriority() == Priority::HIGH);
}

TEST_CASE("editTaskPriority rejects invalid index")
{
    TaskManager manager;

    manager.addTask("Study C++");

    REQUIRE_FALSE(manager.editTaskPriority(500, Priority::MEDIUM));
    REQUIRE(manager.getTasks().at(0).getPriority() == Priority::LOW);    
}

TEST_CASE("markTaskComplete sets task to complete for valid index")
{
    TaskManager manager;

    manager.addTask("Study C++");
    manager.addTask("Buy groceries");
    manager.addTask("Pay bills");

    REQUIRE(manager.markTaskComplete(2));
    REQUIRE(manager.getTasks().at(2).isCompleted());
}

TEST_CASE("markTaskComplete rejects invalid index")
{
    TaskManager manager;

    manager.addTask("Study C++");

    REQUIRE_FALSE(manager.markTaskComplete(500));
    REQUIRE_FALSE(manager.getTasks().at(0).isCompleted());
}

TEST_CASE("markTaskIncomplete works for valid index")
{
    TaskManager manager;

    manager.addTask("Study C++");

    manager.markTaskComplete(0);

    REQUIRE(manager.markTaskIncomplete(0));
    REQUIRE_FALSE(manager.getTasks().at(0).isCompleted());
}

TEST_CASE("markTaskIncomplete rejects invalid index")
{
    TaskManager manager;

    manager.addTask("Study C++");

    manager.markTaskComplete(0);

    REQUIRE_FALSE(manager.markTaskIncomplete(500));
    REQUIRE(manager.getTasks().at(0).isCompleted());
}
