#include "TaskStorage.hpp"

#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

namespace
{
    void writeJsonToFile(
        const std::filesystem::path &path,
        const nlohmann::json &json
    )
    {
        std::ofstream file{path};

        if (!file)
        {
            throw std::runtime_error("Failed to open test file for writing.");
        }

        file << std::setw(4) << json;

        if (!file)
        {
            throw std::runtime_error("Failed to write test file.");
        }
    }
}

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

TEST_CASE("TaskStorage rejects invalid priority")
{
    const std::filesystem::path path{"test_invalid_priority.json"};

    nlohmann::json json_tasks = nlohmann::json::array();

    nlohmann::json json_task;

    json_task["text"] = "Study C++";
    json_task["priority"] = "Urgent";
    json_task["completed"] = false;
    json_task["deadline"] = nullptr;

    json_tasks.push_back(json_task);

    writeJsonToFile(path, json_tasks);

    REQUIRE_THROWS_AS(TaskStorage::load(path), std::invalid_argument);

    std::filesystem::remove(path);
}

TEST_CASE("TaskStorage rejects invalid calendar date")
{
    const std::filesystem::path path{"test_invalid_date.json"};

    nlohmann::json json_tasks = nlohmann::json::array();

    nlohmann::json json_task;

    json_task["text"] = "Study C++";
    json_task["priority"] = "HIGH";
    json_task["completed"] = false;
    json_task["deadline"] = "2026-02-30";

    json_tasks.push_back(json_task);

    writeJsonToFile(path, json_tasks);

    REQUIRE_THROWS_AS(TaskStorage::load(path), std::invalid_argument);

    std::filesystem::remove(path);
}

TEST_CASE("TaskStorage rejects invalid date format")
{
        const std::filesystem::path path{"test_invalid_date.json"};

    nlohmann::json json_tasks = nlohmann::json::array();

    nlohmann::json json_task;

    json_task["text"] = "Study C++";
    json_task["priority"] = "HIGH";
    json_task["completed"] = false;
    json_task["deadline"] = "2026/09/25";

    json_tasks.push_back(json_task);

    writeJsonToFile(path, json_tasks);

    REQUIRE_THROWS_AS(TaskStorage::load(path), std::invalid_argument);

    std::filesystem::remove(path);
}

TEST_CASE("TaskStorage rejects trailing characters in date")
{
            const std::filesystem::path path{"test_invalid_date.json"};

    nlohmann::json json_tasks = nlohmann::json::array();

    nlohmann::json json_task;

    json_task["text"] = "Study C++";
    json_task["priority"] = "HIGH";
    json_task["completed"] = false;
    json_task["deadline"] = "2026-09-25abc";

    json_tasks.push_back(json_task);

   writeJsonToFile(path, json_tasks);

    REQUIRE_THROWS_AS(TaskStorage::load(path), std::invalid_argument);

    std::filesystem::remove(path);
}

TEST_CASE("Missing priority")
{
            const std::filesystem::path path{"test_invalid_date.json"};

    nlohmann::json json_tasks = nlohmann::json::array();

    nlohmann::json json_task;

    json_task["text"] = "Study C++";
    json_task["completed"] = false;
    json_task["deadline"] = "2026/09/25";

    json_tasks.push_back(json_task);

    writeJsonToFile(path, json_tasks);

    REQUIRE_THROWS_AS(TaskStorage::load(path), std::runtime_error);

    std::filesystem::remove(path);
}

TEST_CASE("TaskStorage rejects incorrect field type")
{
                const std::filesystem::path path{"test_invalid_date.json"};

    nlohmann::json json_tasks = nlohmann::json::array();

    nlohmann::json json_task;

    json_task["text"] = "Study C++";
    json_task["priority"] = "HIGH";
    json_task["completed"] = "false";
    json_task["deadline"] = "nullptr";

    json_tasks.push_back(json_task);

   writeJsonToFile(path, json_tasks);

    REQUIRE_THROWS_AS(TaskStorage::load(path), std::runtime_error);

    std::filesystem::remove(path);
}