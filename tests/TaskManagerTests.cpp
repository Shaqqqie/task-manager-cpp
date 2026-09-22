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