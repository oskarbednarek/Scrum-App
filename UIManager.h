#pragma once
#include <string>
#include "imgui.h"

// Forward declarations to avoid circular include dependencies
class AuthenticationManager;
class ProjectManager;
struct Task;
struct UserStory;
struct Sprint;

/**
 * @class UIManager
 * @brief Responsible for rendering the entire user interface using the ImGui library.
 *
 * This class is decoupled from the application logic and handles presentation only.
 */
class UIManager {
public:
    ImFont* headerFont = nullptr; ///< A pointer to a custom font for headers.

    /**
     * @brief The main rendering function, called every frame.
     * @param auth A reference to the authentication manager.
     * @param project A reference to the project manager.
     */
    void RenderUI(AuthenticationManager& auth, ProjectManager& project);

private:
    // --- Methods for rendering specific screens and components ---
    void SetModernTheme();
    void ShowModernLoginScreen(AuthenticationManager& auth);
    void ShowMainUI(AuthenticationManager& auth, ProjectManager& project);

    void ShowDashboard(AuthenticationManager& auth, ProjectManager& project);
    void ShowBacklogWindow(AuthenticationManager& auth, ProjectManager& project);
    void ShowTaskBoardWindow(AuthenticationManager& auth, ProjectManager& project);
    void ShowSprintsWindow(AuthenticationManager& auth, ProjectManager& project);
    void ShowUsersWindow(AuthenticationManager& auth);

    void ShowTaskDetailsPopup(AuthenticationManager& auth, ProjectManager& project);
    void ShowCreateTaskPopup(AuthenticationManager& auth, ProjectManager& project);
    void ShowCreateStoryPopup(AuthenticationManager& auth, ProjectManager& project);

    // --- User Interface State ---
    int currentMainTab = 0;             ///< Index of the active tab in the main UI.
    char loginUsername[64] = "";        ///< Buffer for the username input field on the login screen.
    char loginPassword[64] = "";        ///< Buffer for the password input field.
    std::string loginError;             ///< Login error message to display.

    bool openCreateStoryPopup = false;  ///< Flag to control the visibility of the create story popup.
    bool openCreateTaskPopup = false;   ///< Flag to control the visibility of the create task popup.

    // Buffers for the "Create Story" popup fields
    char newStoryTitle[128] = "";
    char newStoryDesc[256] = "";

    // State and buffers for the "Create Task" popup fields
    int storyIdForNewTask = -1;
    char newTaskTitle[128] = "";
    char newTaskDesc[256] = "";
    char newTaskDeadline[64] = "";
    int newTaskHours = 1;
    int assignedUserIndex = -1;

    // Buffers for the "Create Sprint" fields
    char newSprintName[128] = "";
    char newSprintGoal[256] = "";

    // State and buffers for the "Create User" window
    char newUsername[64] = "";
    char newPassword[64] = "";
    char newFullName[128] = "";
    int newUserRole = 0;
    std::string userCreationError;

    // View-specific state
    int board_viewedSprintId = -1;      ///< The ID of the sprint being viewed on the task board.
    int planning_selectedSprintId = -1; ///< The ID of the sprint selected in the planning panel.
    int details_selectedTaskId = -1;    ///< The ID of the task whose details are currently being viewed.
};