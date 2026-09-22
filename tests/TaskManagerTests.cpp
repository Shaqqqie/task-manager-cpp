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