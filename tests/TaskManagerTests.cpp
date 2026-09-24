#include "TaskManager.hpp"

#include <catch2/catch_test_macros.hpp>

// -----------------------------------------------
// Adding / removing
//------------------------------------------------
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

// -----------------------------------------------
// Editing
//------------------------------------------------

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
        std::chrono::day{23}};

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
        std::chrono::day{23}};

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
        std::chrono::day{31}};
    REQUIRE_THROWS_AS(manager.editTaskDeadline(0, date), std::invalid_argument);
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

// -----------------------------------------------
// Filtering
//------------------------------------------------

TEST_CASE("filterByCompletion filters correctly for completed status")
{
    TaskManager manager;

    manager.addTask("Study C++");
    manager.addTask("Pay bills");
    manager.addTask("Buy groceries");

    manager.markTaskComplete(0);
    manager.markTaskComplete(2);

    auto filtered_by_completion = manager.filterByCompletion(true);

    REQUIRE(filtered_by_completion.size() == 2);

    REQUIRE(filtered_by_completion.at(0).isCompleted());
    REQUIRE(filtered_by_completion.at(0).getText() == "Study C++");

    REQUIRE(filtered_by_completion.at(1).isCompleted());
    REQUIRE(filtered_by_completion.at(1).getText() == "Buy groceries");
}

TEST_CASE("filterByCompletion filters correctly for incomplete status")
{
    TaskManager manager;

    manager.addTask("Study C++");
    manager.addTask("Pay bills");
    manager.addTask("Buy groceries");
    manager.addTask("Gaming with friends");

    manager.markTaskComplete(0);
    manager.markTaskComplete(2);

    auto filtered_by_incompletion = manager.filterByCompletion(false);

    REQUIRE(filtered_by_incompletion.size() == 2);

    REQUIRE_FALSE(filtered_by_incompletion.at(0).isCompleted());
    REQUIRE(filtered_by_incompletion.at(0).getText() == "Pay bills");

    REQUIRE_FALSE(filtered_by_incompletion.at(1).isCompleted());
    REQUIRE(filtered_by_incompletion.at(1).getText() == "Gaming with friends");
}

TEST_CASE("filterByCompletion returns empty for no matches")
{
    TaskManager manager;

    manager.addTask("Study C++");
    manager.addTask("Buy groceries");

    auto filtered_by_completion = manager.filterByCompletion(true);

    REQUIRE(filtered_by_completion.empty());
}

TEST_CASE("filterByPriority filters correctly for given priority's")
{
    TaskManager manager;

    manager.addTask("Study C++");
    manager.addTask("Buy groceries", Priority::MEDIUM);
    manager.addTask("Game with friends", Priority::MEDIUM);
    manager.addTask("Pay bills", Priority::HIGH);

    SECTION("filters correctly for low priority")
    {
        auto filtered_by_priority_low = manager.filterByPriority(Priority::LOW);

        REQUIRE(filtered_by_priority_low.size() == 1);

        REQUIRE(filtered_by_priority_low.at(0).getPriority() == Priority::LOW);
        REQUIRE(filtered_by_priority_low.at(0).getText() == "Study C++");
    }

    SECTION("filters correctly for medium priority")
    {
        auto filtered_by_priority_medium = manager.filterByPriority(Priority::MEDIUM);

        REQUIRE(filtered_by_priority_medium.size() == 2);

        REQUIRE(filtered_by_priority_medium.at(0).getPriority() == Priority::MEDIUM);
        REQUIRE(filtered_by_priority_medium.at(0).getText() == "Buy groceries");

        REQUIRE(filtered_by_priority_medium.at(1).getPriority() == Priority::MEDIUM);
        REQUIRE(filtered_by_priority_medium.at(1).getText() == "Game with friends");
    }

    SECTION("filters correctly for high priority")
    {
        auto filtered_by_priority_high = manager.filterByPriority(Priority::HIGH);

        REQUIRE(filtered_by_priority_high.size() == 1);

        REQUIRE(filtered_by_priority_high.at(0).getPriority() == Priority::HIGH);
        REQUIRE(filtered_by_priority_high.at(0).getText() == "Pay bills");
    }
}

TEST_CASE("filterByPriority returns empty when there are no matches")
{
    TaskManager manager;

    manager.addTask("Study C++");
    manager.addTask("Pay bills");

    auto filtered_by_priority = manager.filterByPriority(Priority::HIGH);

    REQUIRE(filtered_by_priority.empty());
}

TEST_CASE("filterDueToday filters correctly")
{
    SECTION("filters for a specific date")
    {
        TaskManager manager;

        auto today = std::chrono::year{2026} / 9 / 24;
        auto tomorrow = std::chrono::year{2026} / 9 / 25;

        manager.addTask("Study C++", today, Priority::MEDIUM);
        manager.addTask("Buy groceries", today);
        manager.addTask("Pay bills", tomorrow);

        auto filtered_due_today = manager.filterDueToday(today);

        REQUIRE(filtered_due_today.size() == 2);

        REQUIRE(filtered_due_today.at(0).getDeadline() == today);
        REQUIRE(filtered_due_today.at(0).getText() == "Study C++");
        REQUIRE(filtered_due_today.at(0).getPriority() == Priority::MEDIUM);

        REQUIRE(filtered_due_today.at(1).getDeadline() == today);
        REQUIRE(filtered_due_today.at(1).getText() == "Buy groceries");
    }

    SECTION("returns empty for no matching deadline")
    {
        TaskManager manager;

        auto today = std::chrono::year{2026} / 9 / 24;
        auto tomorrow = std::chrono::year{2026} / 9 / 25;

        manager.addTask("Study C++");
        manager.addTask("Buy groceries", tomorrow);

        auto filtered_by_today = manager.filterDueToday(today);

        REQUIRE(filtered_by_today.empty());
    }
}

TEST_CASE("filterOverdue filters correctly")
{
    TaskManager manager;

    auto today = std::chrono::year{2026} / 9 / 24;
    auto test_day = std::chrono::year{2026} / 9 / 20;
    auto tomorrow = std::chrono::year{2026} / 9 / 25;
    auto day_after_tomorrow = std::chrono::year{2026} / 9 / 26;

    manager.addTask("Study C++", test_day);
    manager.addTask("Buy groceries", test_day);
    manager.addTask("Submit assignment", today);
    manager.addTask("Pay bills", tomorrow);
    manager.addTask("Go gym", day_after_tomorrow);
    manager.addTask("Game with friends");

    manager.markTaskComplete(1);

    auto filtered_overdue = manager.filterOverdue(today);

    REQUIRE(filtered_overdue.size() == 1);
    REQUIRE_FALSE(filtered_overdue.at(0).isCompleted());
    REQUIRE(filtered_overdue.at(0).getText() == "Study C++");
    REQUIRE(filtered_overdue.at(0).getDeadline() == test_day);
}

TEST_CASE("filterDueBy filters correctly")
{
    TaskManager manager;

    auto today = std::chrono::year{2026} / 9 / 24;
    auto test_due_date = std::chrono::year{2026} / 9 / 27;
    auto after_test_due_date = std::chrono::year{2026} / 9 / 28;

    manager.addTask("Study C++", today);
    manager.addTask("Buy groceries", today);
    manager.addTask("Pay bills", test_due_date);
    manager.addTask("Submit assignment", after_test_due_date);
    manager.addTask("Go gym");

    manager.markTaskComplete(1);

    auto filtered_by_due_date = manager.filterDueBy(test_due_date);

    REQUIRE(filtered_by_due_date.size() == 2);

    REQUIRE(filtered_by_due_date.at(0).getDeadline() == today);
    REQUIRE(filtered_by_due_date.at(0).getText() == "Study C++");
    REQUIRE_FALSE(filtered_by_due_date.at(0).isCompleted());

    REQUIRE(filtered_by_due_date.at(1).getDeadline() == test_due_date);
    REQUIRE(filtered_by_due_date.at(1).getText() == "Pay bills");
    REQUIRE_FALSE(filtered_by_due_date.at(1).isCompleted());
}

TEST_CASE("filterDueBy throws exception for invalid date")
{
    TaskManager manager;

    auto invalid_date = std::chrono::year{2026} / 2 / 31;

    REQUIRE_THROWS_AS(manager.filterDueBy(invalid_date), std::invalid_argument);
}

TEST_CASE("filterNoDeadline filters correctly")
{
    TaskManager manager;

    auto test_date = std::chrono::year{2026} / 9 / 24;

    manager.addTask("Study C++");
    manager.addTask("Pay bills", test_date);
    manager.addTask("Buy groceries");

    auto filtered_by_no_deadline = manager.filterNoDeadline();

    REQUIRE(filtered_by_no_deadline.size() == 2);

    REQUIRE_FALSE(filtered_by_no_deadline.at(0).getDeadline().has_value());
    REQUIRE(filtered_by_no_deadline.at(0).getText() == "Study C++");

    REQUIRE_FALSE(filtered_by_no_deadline.at(1).getDeadline().has_value());
    REQUIRE(filtered_by_no_deadline.at(1).getText() == "Buy groceries");
}

// -----------------------------------------------
// Sorting
//------------------------------------------------

TEST_CASE("SortByDeadline sorts tasks by ascending deadline")
{
    TaskManager manager;

    auto date1 = std::chrono::year{2026} / 9 / 24;
    auto date2 = std::chrono::year{2026} / 9 / 30;
    auto date3 = std::chrono::year{2026} / 9 / 27;

    manager.addTask("Study C++", date1);
    manager.addTask("Game with friends");
    manager.addTask("Buy groceries", date2);
    manager.addTask("Pay bills", date3);
    manager.addTask("Submit assignment");

    auto sorted_tasks = manager.sortByDeadline();

    REQUIRE(sorted_tasks.size() == 5);
    REQUIRE(sorted_tasks.at(0).getDeadline().value() == date1);
    REQUIRE(sorted_tasks.at(0).getText() == "Study C++");

    REQUIRE(sorted_tasks.at(1).getDeadline().value() == date3);
    REQUIRE(sorted_tasks.at(1).getText() == "Pay bills");

    REQUIRE(sorted_tasks.at(2).getDeadline().value() == date2);
    REQUIRE(sorted_tasks.at(2).getText() == "Buy groceries");

    REQUIRE_FALSE(sorted_tasks.at(3).getDeadline().has_value());
    REQUIRE_FALSE(sorted_tasks.at(4).getDeadline().has_value());

    const auto &original_tasks = manager.getTasks();

    REQUIRE(original_tasks.at(0).getText() == "Study C++");
    REQUIRE(original_tasks.at(1).getText() == "Game with friends");
    REQUIRE(original_tasks.at(2).getText() == "Buy groceries");
    REQUIRE(original_tasks.at(3).getText() == "Pay bills");
    REQUIRE(original_tasks.at(4).getText() == "Submit assignment");
}

TEST_CASE("sortByPriority sorts tasks from high to low priority")
{
    TaskManager manager;

    manager.addTask("Study C++");
    manager.addTask("Buy groceries", Priority::HIGH);
    manager.addTask("Pay bills", Priority::MEDIUM);
    manager.addTask("Game with friends");
    manager.addTask("Submit assignment", Priority::HIGH);

    auto sorted_by_priority = manager.sortByPriority();

    REQUIRE(sorted_by_priority.size() == 5);

    REQUIRE(sorted_by_priority.at(0).getPriority() == Priority::HIGH);
    REQUIRE(sorted_by_priority.at(1).getPriority() == Priority::HIGH);
    REQUIRE(sorted_by_priority.at(2).getPriority() == Priority::MEDIUM);
    REQUIRE(sorted_by_priority.at(3).getPriority() == Priority::LOW);
    REQUIRE(sorted_by_priority.at(4).getPriority() == Priority::LOW);

    const auto &original_tasks = manager.getTasks();

    REQUIRE(original_tasks.at(0).getText() == "Study C++");
    REQUIRE(original_tasks.at(1).getText() == "Buy groceries");
    REQUIRE(original_tasks.at(2).getText() == "Pay bills");
    REQUIRE(original_tasks.at(3).getText() == "Game with friends");
    REQUIRE(original_tasks.at(4).getText() == "Submit assignment");
}