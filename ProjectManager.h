#pragma once
#include "DataTypes.h"
#include <vector>
#include <string>

/**
 * @class ProjectManager
 * @brief The main logic class for managing the project's Scrum artifacts.
 *
 * This class handles the creation, updating, deletion, and retrieval
 * of user stories, tasks, and sprints.
 */
class ProjectManager {
public:
    // User Stories Management
    void CreateUserStory(const std::string& title, const std::string& description, const std::string& creator);
    void UpdateUserStory(const UserStory& updatedStory);
    void DeleteUserStory(int storyId);
    UserStory* GetStoryById(int storyId);
    std::vector<UserStory*> GetUnassignedStories();
    void MoveStoryUp(int storyId);
    void MoveStoryDown(int storyId);

    // Tasks Management
    void CreateTask(int storyId, const std::string& title, const std::string& description, const std::string& creator, const std::string& assignedTo, int estimatedHours, const std::string& deadline);
    void UpdateTask(const Task& updatedTask);
    void DeleteTask(int taskId);
    void UpdateTaskStatus(int taskId, TaskStatus newStatus);
    Task* GetTaskById(int taskId);

    // Sprints & Backlog Management
    void AssignTaskToUser(int taskId, const std::string& username);
    void LogWork(int taskId, const std::string& user, int hours, const std::string& comment);
    void CreateSprint(const std::string& name, const std::string& goal);
    Sprint* GetActiveSprint();
    void UpdateSprint(const Sprint& updatedSprint);
    Sprint* GetSprintById(int sprintId);
    void AssignStoryToSprint(int storyId, int sprintId);
    void RemoveStoryFromSprint(int storyId, int sprintId);
    std::vector<float> GetBurndownData();

    // Public data containers
    std::vector<UserStory> userStories; ///< Vector holding all user stories (the product backlog).
    std::vector<Task> tasks;           ///< Vector holding all tasks.
    std::vector<Sprint> sprints;       ///< Vector holding all sprints.

private:
    /**
     * @brief Generates the next available ID for a given artifact type.
     * @param type The type of artifact ("story", "task", "sprint").
     * @return The next unique ID number.
     */
    int getNextId(const std::string& type);
};