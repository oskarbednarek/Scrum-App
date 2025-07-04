#include "ProjectManager.h"
#include <algorithm>
#include <numeric>

// Function to get the next available ID for a specific type of entity
void ProjectManager::CreateUserStory(const std::string& title, const std::string& description, const std::string& creator) {
    if (title.empty()) return;
    UserStory story;
    story.id = getNextId("story");
    story.title = title;
    story.description = description;
    story.createdBy = creator;
    userStories.push_back(story);
}

std::vector<UserStory*> ProjectManager::GetUnassignedStories() {
    std::vector<UserStory*> unassigned;
    for (auto& story : userStories) {
        bool is_assigned = false;
        for (const auto& sprint : sprints) {
            if (std::find(sprint.storyIds.begin(), sprint.storyIds.end(), story.id) != sprint.storyIds.end()) {
                is_assigned = true;
                break;
            }
        }
        if (!is_assigned) {
            unassigned.push_back(&story);
        }
    }
    return unassigned;
}

// Move a user story up in the list
void ProjectManager::MoveStoryUp(int storyId) {
    auto it = std::find_if(userStories.begin(), userStories.end(), [storyId](const UserStory& story) {
        return story.id == storyId;
    });

    if (it != userStories.begin() && it != userStories.end()) {
        std::iter_swap(it, it - 1);
    }
}

void ProjectManager::MoveStoryDown(int storyId) {
    auto it = std::find_if(userStories.begin(), userStories.end(), [storyId](const UserStory& story) {
        return story.id == storyId;
    });

    if (it != userStories.end() && (it + 1) != userStories.end()) {
        std::iter_swap(it, it + 1);
    }
}

void ProjectManager::UpdateUserStory(const UserStory& updatedStory) {
    auto it = std::find_if(userStories.begin(), userStories.end(), [&](const UserStory& s) {
        return s.id == updatedStory.id;
    });
    if (it != userStories.end()) {
        *it = updatedStory;
    }
}
void ProjectManager::DeleteUserStory(int storyId) {
    userStories.erase(std::remove_if(userStories.begin(), userStories.end(), [&](const UserStory& story) {
        return story.id == storyId;
    }), userStories.end());
    tasks.erase(std::remove_if(tasks.begin(), tasks.end(), [&](const Task& task){
        return task.storyId == storyId;
    }), tasks.end());
}
UserStory* ProjectManager::GetStoryById(int storyId) {
    auto it = std::find_if(userStories.begin(), userStories.end(), [&](const UserStory& s) { return s.id == storyId; });
    return (it != userStories.end()) ? &(*it) : nullptr;
}
void ProjectManager::CreateTask(int storyId, const std::string& title, const std::string& description, const std::string& creator, const std::string& assignedTo, int estimatedHours, const std::string& deadline) {
    if (title.empty()) return;
    Task task;
    task.id = getNextId("task");
    task.storyId = storyId;
    task.title = title;
    task.description = description;
    task.createdBy = creator;
    task.assignedTo = assignedTo;
    task.estimatedHours = estimatedHours;
    task.deadline = deadline;
    tasks.push_back(task);
}
void ProjectManager::UpdateTask(const Task& updatedTask) {
    auto it = std::find_if(tasks.begin(), tasks.end(), [&](const Task& t) {
        return t.id == updatedTask.id;
    });
    if (it != tasks.end()) {
        *it = updatedTask;
    }
}
void ProjectManager::DeleteTask(int taskId) {
    tasks.erase(std::remove_if(tasks.begin(), tasks.end(), [&](const Task& t) { return t.id == taskId; }), tasks.end());
}
void ProjectManager::UpdateTaskStatus(int taskId, TaskStatus newStatus) {
    Task* task = GetTaskById(taskId);
    if (task) task->status = newStatus;
}
Task* ProjectManager::GetTaskById(int taskId) {
    auto it = std::find_if(tasks.begin(), tasks.end(), [&](const Task& t) { return t.id == taskId; });
    return (it != tasks.end()) ? &(*it) : nullptr;
}
void ProjectManager::AssignTaskToUser(int taskId, const std::string& username) {
    Task* task = GetTaskById(taskId);
    if (task) {
        task->assignedTo = username;
    }
}
void ProjectManager::LogWork(int taskId, const std::string& user, int hours, const std::string& comment) {
    Task* task = GetTaskById(taskId);
    if (task) {
        task->loggedHours += hours;
        std::string log_entry = "User: " + user + ", Hours: " + std::to_string(hours) + " - " + comment;
        task->workLog.push_back(log_entry);
    }
}
void ProjectManager::CreateSprint(const std::string& name, const std::string& goal) {
    if (name.empty()) return;
    Sprint sprint;
    sprint.id = getNextId("sprint");
    sprint.name = name;
    sprint.goal = goal;
    sprints.push_back(sprint);
}
Sprint* ProjectManager::GetActiveSprint() {
    auto it = std::find_if(sprints.begin(), sprints.end(), [](const Sprint& s) {
        return s.status == SprintStatus::ACTIVE;
    });
    return (it != sprints.end()) ? &(*it) : nullptr;
}
void ProjectManager::UpdateSprint(const Sprint& updatedSprint) {
    auto it = std::find_if(sprints.begin(), sprints.end(), [&](const Sprint& s) {
        return s.id == updatedSprint.id;
    });
    if (it != sprints.end()) {
        *it = updatedSprint;
    }
}
Sprint* ProjectManager::GetSprintById(int sprintId) {
    auto it = std::find_if(sprints.begin(), sprints.end(), [&](const Sprint& s) { return s.id == sprintId; });
    return (it != sprints.end()) ? &(*it) : nullptr;
}
void ProjectManager::AssignStoryToSprint(int storyId, int sprintId) {
    Sprint* sprint = GetSprintById(sprintId);
    if (sprint && sprint->status == SprintStatus::PLANNING) {
        for (auto& s : sprints) {
            s.storyIds.erase(std::remove(s.storyIds.begin(), s.storyIds.end(), storyId), s.storyIds.end());
        }
        sprint->storyIds.push_back(storyId);
    }
}
void ProjectManager::RemoveStoryFromSprint(int storyId, int sprintId) {
    Sprint* sprint = GetSprintById(sprintId);
    if (sprint) {
        sprint->storyIds.erase(std::remove(sprint->storyIds.begin(), sprint->storyIds.end(), storyId), sprint->storyIds.end());
    }
}
std::vector<float> ProjectManager::GetBurndownData() {
    Sprint* activeSprint = GetActiveSprint();
    if (!activeSprint) return {};
    std::vector<Task> sprintTasks;
    for (int storyId : activeSprint->storyIds) {
        for (const auto& task : tasks) {
            if (task.storyId == storyId) {
                sprintTasks.push_back(task);
            }
        }
    }
    if (sprintTasks.empty()) return {};
    int totalHours = 0;
    for (const auto& task : sprintTasks) {
        totalHours += task.estimatedHours;
    }
    std::vector<float> data;
    data.push_back(static_cast<float>(totalHours));
    std::sort(sprintTasks.begin(), sprintTasks.end(), [](const Task& a, const Task& b) {
        return a.status < b.status;
    });
    int remainingHours = totalHours;
    for (const auto& task : sprintTasks) {
        if (task.status == TaskStatus::DONE) {
            remainingHours -= task.estimatedHours;
        }
        data.push_back(static_cast<float>(remainingHours));
    }
    return data;
}
int ProjectManager::getNextId(const std::string& type) {
    int maxId = 0;
    if (type == "story" && !userStories.empty()) {
        for (const auto& s : userStories) maxId = std::max(maxId, s.id);
    } else if (type == "task" && !tasks.empty()) {
        for (const auto& t : tasks) maxId = std::max(maxId, t.id);
    } else if (type == "sprint" && !sprints.empty()) {
        for (const auto& s : sprints) maxId = std::max(maxId, s.id);
    }
    return maxId + 1;
}