#pragma once
#include "DataTypes.h"
#include <vector>
#include <string>

/**
 * @class DataManager
 * @brief This class is solely responsible for reading from and writing to text files.
 *
 * It encapsulates all file I/O logic, separating data persistence
 * from the rest of the application's logic.
 */
class DataManager {
public:
    /**
     * @brief Loads all data (users, stories, tasks, sprints) from files.
     * @param users A reference to the vector where users will be loaded.
     * @param stories A reference to the vector where stories will be loaded.
     * @param tasks A reference to the vector where tasks will be loaded.
     * @param sprints A reference to the vector where sprints will be loaded.
     */
    void LoadAll(std::vector<User>& users, std::vector<UserStory>& stories, std::vector<Task>& tasks, std::vector<Sprint>& sprints);

    /**
     * @brief Saves all data (users, stories, tasks, sprints) to files.
     * @param users A const reference to the vector of users to be saved.
     * @param stories A const reference to the vector of stories to be saved.
     * @param tasks A const reference to the vector of tasks to be saved.
     * @param sprints A const reference to the vector of sprints to be saved.
     */
    void SaveAll(const std::vector<User>& users, const std::vector<UserStory>& stories, const std::vector<Task>& tasks, const std::vector<Sprint>& sprints);

private:
    // Declarations for private methods handling specific data types
    void LoadUsers(std::vector<User>& users);
    void SaveUsers(const std::vector<User>& users);
    void CreateDefaultUsers(std::vector<User>& users);

    void LoadUserStories(std::vector<UserStory>& stories);
    void SaveUserStories(const std::vector<UserStory>& stories);

    void LoadTasks(std::vector<Task>& tasks);
    void SaveTasks(const std::vector<Task>& tasks);

    void LoadSprints(std::vector<Sprint>& sprints);
    void SaveSprints(const std::vector<Sprint>& sprints);
};