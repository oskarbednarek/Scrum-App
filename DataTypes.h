#pragma once

#include <string>
#include <vector>
#include <iostream>

class ScrumArtifact {
public:
    int id = 0;
    std::string title;
    std::string description;
    std::string createdBy;

    virtual std::string getDetails() const = 0;
    virtual ~ScrumArtifact() = default;
};

enum class UserRole { DEVELOPER, SCRUM_MASTER, PRODUCT_OWNER, TESTER };
enum class TaskStatus { TODO, IN_PROGRESS, TESTING, DONE };
enum class StoryPriority { LOW, MEDIUM, HIGH, CRITICAL };
enum class SprintStatus { PLANNING, ACTIVE, COMPLETED };

struct User {
    std::string username;
    std::string password;
    std::string fullName;
    UserRole role;

    bool operator==(const User& other) const {
        return username == other.username;
    }
};

struct UserStory : public ScrumArtifact {
    std::string acceptanceCriteria;
    StoryPriority priority = StoryPriority::MEDIUM;
    int storyPoints = 1;
    bool isCompleted = false;

    std::string getDetails() const override {
        return "[STORY #" + std::to_string(id) + "] " + title;
    }
};

struct Task : public ScrumArtifact {
    int storyId = 0;
    TaskStatus status = TaskStatus::TODO;
    std::string assignedTo;
    int estimatedHours = 1;
    int loggedHours = 0;
    std::string deadline;
    std::vector<std::string> workLog;

    std::string getDetails() const override {
        return "[TASK #" + std::to_string(id) + "] " + title;
    }
};

struct Sprint {
    int id = 0;
    std::string name;
    std::string goal;
    SprintStatus status = SprintStatus::PLANNING;
    std::vector<int> storyIds;
    std::string startDate;
    std::string endDate;
};

inline const char* RoleToString(UserRole role) {
    switch (role) {
        case UserRole::DEVELOPER: return "Developer";
        case UserRole::SCRUM_MASTER: return "Scrum Master";
        case UserRole::PRODUCT_OWNER: return "Product Owner";
        case UserRole::TESTER: return "Tester";
        default: return "Unknown";
    }
}

inline const char* PriorityToString(StoryPriority p) {
    switch (p) {
        case StoryPriority::LOW: return "Low";
        case StoryPriority::MEDIUM: return "Medium";
        case StoryPriority::HIGH: return "High";
        case StoryPriority::CRITICAL: return "Critical";
        default: return "Unknown";
    }
}

inline const char* TaskStatusToString(TaskStatus s) {
    switch (s) {
        case TaskStatus::TODO: return "To Do";
        case TaskStatus::IN_PROGRESS: return "In Progress";
        case TaskStatus::TESTING: return "Testing";
        case TaskStatus::DONE: return "Done";
        default: return "Unknown";
    }
}

inline const char* SprintStatusToString(SprintStatus s) {
    switch (s) {
        case SprintStatus::PLANNING: return "Planning";
        case SprintStatus::ACTIVE: return "Active";
        case SprintStatus::COMPLETED: return "Completed";
        default: return "Unknown";
    }
}