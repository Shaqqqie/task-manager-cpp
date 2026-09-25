#include "TaskStorage.hpp"

#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

TEST_CASE("TaskStorage saves tasks to a file")
{
    const std::filesystem::path path{"test_tasks.json"};

    std::vector<Task> tasks{};
    tasks.emplace_back("Study C++", Priority::HIGH);

    TaskStorage::save(tasks, path);

    REQUIRE(std::filesystem::exists(path));

    {
        std::ifstream file{path};
        REQUIRE(file);

        nlohmann::json json;
        file >> json;

        REQUIRE(json.size() == 1);
        REQUIRE(json.at(0).at("text") == "Study C++");
        REQUIRE(json.at(0).at("priority") == "HIGH");
        REQUIRE_FALSE(json.at(0).at("completed").get<bool>());
        REQUIRE(json.at(0).at("deadline").is_null());
    }

    std::filesystem::remove(path);
}

TEST_CASE("TaskStorage saves task with deadline correctly")
{
    const std::chrono::year_month_day deadline{
        std::chrono::year{2026},
        std::chrono::month{9},
        std::chrono::day{25}};

    const std::filesystem::path path{"test_tasks.json"};

    std::vector<Task> tasks{};
    tasks.emplace_back("Finish project", deadline, Priority::MEDIUM);

    TaskStorage::save(tasks, path);

    REQUIRE(std::filesystem::exists(path));

    {
        std::ifstream file{path};
        REQUIRE(file);

        nlohmann::json json;
        file >> json;

        REQUIRE(json.size() == 1);
        REQUIRE(json.at(0).at("text") == "Finish project");
        REQUIRE_FALSE(json.at(0).at("completed").get<bool>());
        REQUIRE(json.at(0).at("priority") == "MEDIUM");
        REQUIRE(json.at(0).at("deadline") == "2026-09-25");
    }

    std::filesystem::remove(path);
}

TEST_CASE("TaskStorage saves and loads tasks")
{
    const std::filesystem::path path{"test_tasks.json"};

    const std::chrono::year_month_day deadline{
        std::chrono::year{2026},
        std::chrono::month{9},
        std::chrono::day{25}};

    std::vector<Task> original_tasks{};

    original_tasks.emplace_back(
        "Finish project",
        deadline,
        Priority::HIGH);

    original_tasks.emplace_back(
        "Buy groceries",
        Priority::MEDIUM);

    original_tasks.at(0).markComplete();

    TaskStorage::save(original_tasks, path);

    std::vector<Task> loaded_tasks{
        TaskStorage::load(path)};

    REQUIRE(loaded_tasks.size() == 2);

    REQUIRE(loaded_tasks.at(0).getText() == "Finish project");
    REQUIRE(loaded_tasks.at(0).getDeadline().value() == deadline);
    REQUIRE(loaded_tasks.at(0).getPriority() == Priority::HIGH);
    REQUIRE(loaded_tasks.at(0).isCompleted());

    REQUIRE(loaded_tasks.at(1).getText() == "Buy groceries");
    REQUIRE_FALSE(loaded_tasks.at(1).getDeadline().has_value());
    REQUIRE(loaded_tasks.at(1).getPriority() == Priority::MEDIUM);
    REQUIRE_FALSE(loaded_tasks.at(1).isCompleted());

    std::filesystem::remove(path);
}