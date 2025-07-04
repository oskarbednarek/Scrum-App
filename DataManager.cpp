#include "DataManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

#define GET_LINE_OR_CONTINUE(stream, var) if (!std::getline(stream, var, '|')) continue;

// Methods to load and save data for tasks, user stories, users, and sprints

void DataManager::LoadTasks(std::vector<Task>& tasks) {
    tasks.clear();
    std::ifstream file("tasks.txt");
    if (!file.is_open()) return;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        Task task;
        std::string idStr, storyIdStr, statusStr, estHoursStr, loggedHoursStr;
        GET_LINE_OR_CONTINUE(ss, idStr);
        GET_LINE_OR_CONTINUE(ss, storyIdStr);
        GET_LINE_OR_CONTINUE(ss, task.title);
        GET_LINE_OR_CONTINUE(ss, task.description);
        GET_LINE_OR_CONTINUE(ss, statusStr);
        GET_LINE_OR_CONTINUE(ss, task.assignedTo);
        GET_LINE_OR_CONTINUE(ss, task.createdBy);
        GET_LINE_OR_CONTINUE(ss, estHoursStr);
        GET_LINE_OR_CONTINUE(ss, loggedHoursStr);
        std::getline(ss, task.deadline);

        task.id = std::stoi(idStr);
        task.storyId = std::stoi(storyIdStr);
        task.status = static_cast<TaskStatus>(std::stoi(statusStr));
        task.estimatedHours = std::stoi(estHoursStr);
        task.loggedHours = std::stoi(loggedHoursStr);
        tasks.push_back(task);
    }
    file.close();
}

void DataManager::SaveTasks(const std::vector<Task>& tasks) {
    std::ofstream file("tasks.txt");
    for (const auto& task : tasks) {
        file << task.id << "|" << task.storyId << "|" << task.title << "|" << task.description << "|"
             << static_cast<int>(task.status) << "|" << task.assignedTo << "|" << task.createdBy << "|"
             << task.estimatedHours << "|" << task.loggedHours << "|" << task.deadline << "\n"; // **NEW**
    }
    file.close();
}

void DataManager::LoadAll(std::vector<User>& users, std::vector<UserStory>& stories, std::vector<Task>& tasks, std::vector<Sprint>& sprints) {
    LoadUsers(users);
    LoadUserStories(stories);
    LoadTasks(tasks);
    LoadSprints(sprints);
}

void DataManager::SaveAll(const std::vector<User>& users, const std::vector<UserStory>& stories, const std::vector<Task>& tasks, const std::vector<Sprint>& sprints) {
    SaveUsers(users);
    SaveUserStories(stories);
    SaveTasks(tasks);
    SaveSprints(sprints);
}

void DataManager::LoadUsers(std::vector<User>& users) {
    users.clear();
    std::ifstream file("users.txt");
    if (!file.is_open()) {
        CreateDefaultUsers(users);
        SaveUsers(users);
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string username, password, fullName, roleStr;
        GET_LINE_OR_CONTINUE(ss, username);
        GET_LINE_OR_CONTINUE(ss, password);
        GET_LINE_OR_CONTINUE(ss, fullName);
        if (!std::getline(ss, roleStr)) continue; // Last field doesn't need a '|'
        users.push_back({username, password, fullName, static_cast<UserRole>(std::stoi(roleStr))});
    }
    file.close();
}

void DataManager::SaveUsers(const std::vector<User>& users) {
    std::ofstream file("users.txt");
    for (const auto& user : users) {
        file << user.username << "|" << user.password << "|" << user.fullName << "|" << static_cast<int>(user.role) << "\n";
    }
    file.close();
}

void DataManager::CreateDefaultUsers(std::vector<User>& users) {
    users = {
        {"admin", "admin", "Admin (Scrum Master)", UserRole::SCRUM_MASTER},
        {"po", "po", "Product Owner", UserRole::PRODUCT_OWNER},
        {"dev", "dev", "Developer", UserRole::DEVELOPER}
    };
}

void DataManager::LoadUserStories(std::vector<UserStory>& stories) {
    stories.clear();
    std::ifstream file("stories.txt");
    if (!file.is_open()) return;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        UserStory story;
        std::string idStr, priorityStr, pointsStr, completedStr;
        GET_LINE_OR_CONTINUE(ss, idStr);
        GET_LINE_OR_CONTINUE(ss, story.title);
        GET_LINE_OR_CONTINUE(ss, story.description);
        GET_LINE_OR_CONTINUE(ss, story.acceptanceCriteria);
        GET_LINE_OR_CONTINUE(ss, priorityStr);
        GET_LINE_OR_CONTINUE(ss, pointsStr);
        GET_LINE_OR_CONTINUE(ss, story.createdBy);
        if(!std::getline(ss, completedStr)) continue;

        story.id = std::stoi(idStr);
        story.priority = static_cast<StoryPriority>(std::stoi(priorityStr));
        story.storyPoints = std::stoi(pointsStr);
        story.isCompleted = (completedStr == "1");
        stories.push_back(story);
    }
    file.close();
}

void DataManager::SaveUserStories(const std::vector<UserStory>& stories) {
    std::ofstream file("stories.txt");
    for (const auto& story : stories) {
        file << story.id << "|" << story.title << "|" << story.description << "|"
             << story.acceptanceCriteria << "|" << static_cast<int>(story.priority) << "|"
             << story.storyPoints << "|" << story.createdBy << "|" << (story.isCompleted ? "1" : "0") << "\n";
    }
    file.close();
}

void DataManager::LoadSprints(std::vector<Sprint>& sprints) {
    sprints.clear();
    std::ifstream file("sprints.txt");
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        Sprint sprint;
        std::string idStr, statusStr;

        GET_LINE_OR_CONTINUE(ss, idStr);
        GET_LINE_OR_CONTINUE(ss, sprint.name);
        GET_LINE_OR_CONTINUE(ss, statusStr);
        GET_LINE_OR_CONTINUE(ss, sprint.startDate);
        if (!std::getline(ss, sprint.endDate)) continue;

        sprint.id = std::stoi(idStr);
        sprint.status = static_cast<SprintStatus>(std::stoi(statusStr));
        sprints.push_back(sprint);
    }
    file.close();
}

void DataManager::SaveSprints(const std::vector<Sprint>& sprints) {
    std::ofstream file("sprints.txt");
    for (const auto& sprint : sprints) {
        file << sprint.id << "|" << sprint.name << "|" << static_cast<int>(sprint.status) << "|"
             << sprint.startDate << "|" << sprint.endDate << "\n";
    }
    file.close();
}