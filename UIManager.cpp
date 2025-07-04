#include "UIManager.h"
#include "AuthenticationManager.h"
#include "ProjectManager.h"
#include "DataTypes.h"
#include "imgui.h"
#include <string>
#include <vector>
#include <algorithm>
#include <cstdio>

// Main entry point for rendering UI
void UIManager::RenderUI(AuthenticationManager& auth, ProjectManager& project) {
    static bool themeSet = false;
    if (!themeSet) {
        SetModernTheme();
        themeSet = true;
    }

    if (!auth.IsLoggedIn()) {
        ShowModernLoginScreen(auth);
    } else {
        ShowMainUI(auth, project);
    }

    // Modal popups are rendered on top, outside the main window flow.
    if (openCreateStoryPopup) ShowCreateStoryPopup(auth, project);
    if (openCreateTaskPopup) ShowCreateTaskPopup(auth, project);
    if (details_selectedTaskId != -1) ShowTaskDetailsPopup(auth, project);
}

// different theme
// naming it modern just to differentiate from the default ImGui theme
void UIManager::SetModernTheme() {
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.10f, 0.10f, 0.11f, 1.00f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.14f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border]                 = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.24f, 0.24f, 0.26f, 1.00f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.28f, 0.28f, 0.31f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.14f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.14f, 0.44f, 0.44f, 0.76f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.14f, 0.44f, 0.44f, 0.86f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.14f, 0.44f, 0.44f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.14f, 0.44f, 0.44f, 0.76f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.14f, 0.44f, 0.44f, 0.86f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.14f, 0.44f, 0.44f, 1.00f);
    colors[ImGuiCol_Separator]              = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 6.0f;
    style.ChildRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.PopupRounding = 6.0f;
}

//Login Screen
void UIManager::ShowModernLoginScreen(AuthenticationManager& auth) {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("LoginScreenWindow", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    ImGui::Columns(2, "LoginScreen", false);
    ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() * 0.35f);

    // Left Panel
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.14f, 0.15f, 0.17f, 1.00f));
    ImGui::BeginChild("Branding", ImVec2(0, 0), false); // No border needed
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() * 0.3f);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
    if(headerFont) ImGui::PushFont(headerFont);
    ImGui::Text("Scrum App");
    if(headerFont) ImGui::PopFont();
    ImGui::Text("Made by Oskar Bednarek");
    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::NextColumn();

    // Right Panel
    float formWidth = 300.0f;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x - formWidth) * 0.5f);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (ImGui::GetContentRegionAvail().y - 200.0f) * 0.5f);

    ImGui::BeginChild("LoginForm", ImVec2(formWidth, 200.0f), false, ImGuiWindowFlags_NoBackground);
    if(headerFont) ImGui::PushFont(headerFont);
    ImGui::Text("Welcome Back!");
    if(headerFont) ImGui::PopFont();
    ImGui::Spacing();

    ImGui::PushItemWidth(-1);
    ImGui::InputText("Username", loginUsername, sizeof(loginUsername));
    ImGui::InputText("Password", loginPassword, sizeof(loginPassword), ImGuiInputTextFlags_Password);
    ImGui::PopItemWidth();
    ImGui::Spacing();

    if (ImGui::Button("Login", ImVec2(-1, 30))) {
        if (!auth.Login(loginUsername, loginPassword)) {
            loginError = "Invalid credentials.";
        } else {
            loginError = "";
        }
    }
    if (!loginError.empty()) {
        ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "%s", loginError.c_str());
    }
    ImGui::EndChild();

    ImGui::Columns(1);
    ImGui::End(); // End LoginScreenWindow
}

// Main application UI
void UIManager::ShowMainUI(AuthenticationManager& auth, ProjectManager& project) {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("MainAppWindow", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    ImGui::Columns(2, "MainUI", false);
    ImGui::SetColumnWidth(0, 220.0f);

    // --- Left Panel: Navigation ---
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.14f, 0.15f, 0.17f, 1.00f));
    ImGui::BeginChild("Navigation", ImVec2(0,0), false);

    ImGui::SetCursorPosY(20);
    ImGui::SetCursorPosX(10);
    if(headerFont) ImGui::PushFont(headerFont);
    ImGui::Text("Scrum App");
    if(headerFont) ImGui::PopFont();
    ImGui::Separator();

    const char* menuItems[] = {"[D] Dashboard", "[B] Product Backlog", "[T] Task Board", "[S] Sprint Planning", "[U] User Management"};    bool canAccess[] = {true, true, true, auth.CanManageSprints(), auth.CanManageUsers()};

    ImGui::SetCursorPosX(10);
    ImGui::BeginChild("MenuItems", ImVec2(0, ImGui::GetWindowHeight() - 100));
    for(int i = 0; i < 5; ++i) {
        if(canAccess[i]) {
            if (ImGui::Selectable(menuItems[i], currentMainTab == i)) { currentMainTab = i; }
        }
    }
    ImGui::EndChild();

    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 60.0f);
    ImGui::SetCursorPosX(10);
    ImGui::Separator();
    const auto* user = auth.GetCurrentUser();
    ImGui::Text("%s", user->fullName.c_str());
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "%s", RoleToString(user->role));
    if (ImGui::SmallButton("Logout")) { auth.Logout(); }

    ImGui::EndChild();
    ImGui::PopStyleColor();

    // --- Right Panel: Content ---
    ImGui::NextColumn();
    ImGui::BeginChild("Content", ImVec2(0,0), false);
    switch (currentMainTab) {
        case 0: ShowDashboard(auth, project); break;
        case 1: ShowBacklogWindow(auth, project); break;
        case 2: ShowTaskBoardWindow(auth, project); break;
        case 3: if (auth.CanManageSprints()) ShowSprintsWindow(auth, project); break;
        case 4: if (auth.CanManageUsers()) ShowUsersWindow(auth); break;
    }
    ImGui::EndChild();

    ImGui::Columns(1);
    ImGui::End();
}

// Dashboard UI
void UIManager::ShowDashboard(AuthenticationManager& auth, ProjectManager& project) {
    if(headerFont) ImGui::PushFont(headerFont);
    ImGui::Text("Dashboard");
    if(headerFont) ImGui::PopFont();
    ImGui::Separator();

    Sprint* activeSprint = project.GetActiveSprint();
    if (activeSprint) {
        ImGui::Text("Active Sprint: %s", activeSprint->name.c_str());
        ImGui::Text("Goal: %s", activeSprint->goal.c_str());
        ImGui::Separator();
        ImGui::Text("Burndown Chart");
        std::vector<float> data = project.GetBurndownData();
        if (data.size() > 1) {
            ImGui::PlotLines("##Burndown", data.data(), data.size(), 0, "Progress", 0.0f, *std::max_element(data.begin(), data.end()), ImVec2(-1, 150));
        } else {
            ImGui::Text("Not enough data to display a chart.");
        }
    } else {
        ImGui::Text("There is no active sprint.");
    }

    ImGui::Separator();

    ImGui::Text("My Assigned Tasks");
    ImGui::BeginChild("MyTasks", ImVec2(0, 150), true);
    bool foundTask = false;
    for (const auto& task : project.tasks) {
        if (task.assignedTo == auth.GetCurrentUser()->username && task.status != TaskStatus::DONE) {
            ImGui::BulletText("%s (in Story #%d)", task.title.c_str(), task.storyId);
            foundTask = true;
        }
    }
    if (!foundTask) {
        ImGui::Text("No active tasks assigned to you.");
    }
    ImGui::EndChild();
}

void UIManager::ShowBacklogWindow(AuthenticationManager& auth, ProjectManager& project) {
    if(headerFont) ImGui::PushFont(headerFont);
    ImGui::Text("Product Backlog");
    if(headerFont) ImGui::PopFont();
    ImGui::Separator();

    if (auth.CanPerformAction(UserRole::PRODUCT_OWNER)) {
        if (ImGui::Button("[+] Add New User Story")) {
            openCreateStoryPopup = true;
        }
        ImGui::Separator();
    }

    for (size_t i = 0; i < project.userStories.size(); ++i) {
        auto& story = project.userStories[i];
        ImGui::PushID(story.id);

        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
        ImGui::BeginChild("StoryCard", ImVec2(0, 120), true, ImGuiWindowFlags_NoScrollbar);

        ImGui::Columns(2, "story_card_cols", false);
        ImGui::SetColumnWidth(0, 40.0f);

        if (auth.CanPerformAction(UserRole::PRODUCT_OWNER)) {
            if (ImGui::Button("^", ImVec2(25, 25))) { project.MoveStoryUp(story.id); ImGui::Columns(1); ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopID(); break; }
            if (ImGui::Button("v", ImVec2(25, 25))) { project.MoveStoryDown(story.id); ImGui::Columns(1); ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopID(); break; }
        }
        ImGui::NextColumn();

        ImGui::Text("ID %d: %s", story.id, story.title.c_str());
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Points: %d", story.storyPoints);
        ImGui::TextWrapped("%s", story.description.c_str());

        float buttonPosX = ImGui::GetColumnWidth() - 120.0f;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + buttonPosX);
        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 35.0f);
        if (ImGui::Button("[+] Add Task", ImVec2(100, 25))) {
            storyIdForNewTask = story.id;
            openCreateTaskPopup = true;
        }

        ImGui::Columns(1);
        ImGui::EndChild();
        ImGui::PopStyleVar();
        ImGui::Spacing();

        ImGui::PopID();
    }
}

//Task Board UI
void UIManager::ShowTaskBoardWindow(AuthenticationManager& auth, ProjectManager& project) {
    if(headerFont) ImGui::PushFont(headerFont);
    ImGui::Text("Task Board");
    if(headerFont) ImGui::PopFont();

    Sprint* tempSprint = project.GetSprintById(board_viewedSprintId);
    const char* preview_text = tempSprint ? tempSprint->name.c_str() : "Select a Sprint to view...";
    ImGui::PushItemWidth(300);
    if (ImGui::BeginCombo("View Sprint", preview_text)) {
        for (const auto& sprint : project.sprints) {
            if (ImGui::Selectable(sprint.name.c_str(), board_viewedSprintId == sprint.id)) {
                board_viewedSprintId = sprint.id;
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();
    ImGui::Separator();

    const int numColumns = 4;
    const char* columnNames[] = {"To Do", "In Progress", "Testing", "Done"};
    const TaskStatus columnStatuses[] = {TaskStatus::TODO, TaskStatus::IN_PROGRESS, TaskStatus::TESTING, TaskStatus::DONE};
    ImGui::Columns(numColumns, "task_board_columns", false);

    Sprint* viewedSprint = project.GetSprintById(board_viewedSprintId);
    for (int i = 0; i < numColumns; i++) {
        ImGui::Text("%s", columnNames[i]);
        ImGui::Separator();

        ImGui::BeginChild(columnNames[i], ImVec2(ImGui::GetColumnWidth() - 10, 0), false, ImGuiWindowFlags_NoScrollbar);
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TASK_DRAG_N_DROP")) {
                int taskId = *(const int*)payload->Data;
                project.UpdateTaskStatus(taskId, columnStatuses[i]);
            }
            ImGui::EndDragDropTarget();
        }

        if (viewedSprint) {
            for (auto& task : project.tasks) {
                bool inSelectedSprint = std::any_of(viewedSprint->storyIds.begin(), viewedSprint->storyIds.end(),
                                                  [&](int storyId){ return task.storyId == storyId; });
                if (inSelectedSprint && task.status == columnStatuses[i]) {
                    ImGui::PushID(task.id);

                    ImVec4 bgColor = ImVec4(0.25f, 0.25f, 0.27f, 1.0f);
                    if (ImGui::IsItemHovered()) bgColor.w = 1.2f;
                    if (task.status == TaskStatus::IN_PROGRESS) bgColor = ImVec4(0.2f, 0.2f, 0.4f, 1.0f);
                    else if (task.status == TaskStatus::TESTING) bgColor = ImVec4(0.4f, 0.4f, 0.2f, 1.0f);
                    else if (task.status == TaskStatus::DONE) bgColor = ImVec4(0.2f, 0.4f, 0.2f, 1.0f);

                    ImGui::PushStyleColor(ImGuiCol_Button, bgColor);
                    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.05f, 0.2f));
                    if (ImGui::Button(task.title.c_str(), ImVec2(-1, 100))) {
                        details_selectedTaskId = task.id;
                    }
                    ImGui::PopStyleVar(1);

                    if (ImGui::IsItemHovered()){ ImGui::SetTooltip("Click for details or drag to change status"); }
                    ImGui::PopStyleColor(1);

                    if (ImGui::BeginDragDropSource()) {
                        ImGui::SetDragDropPayload("TASK_DRAG_N_DROP", &task.id, sizeof(int));
                        ImGui::Text("Move: %s", task.title.c_str());
                        ImGui::EndDragDropSource();
                    }

                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 60);
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                    ImGui::Text("To: %s", task.assignedTo.empty() ? "N/A" : task.assignedTo.c_str());

                    if (!task.deadline.empty()) {
                        std::string today_str = "2025-06-17";
                        ImVec4 deadline_color = ImVec4(1.0f, 0.8f, 0.4f, 1.0f); // Default amber color

                        if (task.deadline < today_str) {
                            deadline_color = ImVec4(1.0f, 0.2f, 0.2f, 1.0f); // Overdue = RED
                        }

                        ImGui::TextColored(deadline_color, "Due: %s", task.deadline.c_str());
                    }

                    ImGui::Spacing();
                    ImGui::PopID();
                }
            }
        }
        ImGui::EndChild();
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
}

// Sprint Planning UI
void UIManager::ShowSprintsWindow(AuthenticationManager& auth, ProjectManager& project) {
    if(headerFont) ImGui::PushFont(headerFont);
    ImGui::Text("Sprint Planning");
    if(headerFont) ImGui::PopFont();
    ImGui::Separator();

    ImGui::Columns(2, "sprint_planning_view", false);
    ImGui::SetColumnWidth(0, 250);

    ImGui::BeginChild("SprintControls");
    ImGui::Text("Sprints");
    ImGui::Separator();
    ImGui::BeginChild("SprintList", ImVec2(0, ImGui::GetContentRegionAvail().y - 130), true);
    for (auto& sprint : project.sprints) {
        if (ImGui::Selectable(sprint.name.c_str(), planning_selectedSprintId == sprint.id)) {
            planning_selectedSprintId = sprint.id;
        }
    }
    ImGui::EndChild();

    ImGui::Separator();
    ImGui::InputText("Sprint Name", newSprintName, sizeof(newSprintName));
    ImGui::InputText("Sprint Goal", newSprintGoal, sizeof(newSprintGoal));
    if (ImGui::Button("[+] Create Sprint", ImVec2(-1, 0))) {
        project.CreateSprint(newSprintName, newSprintGoal);
        newSprintName[0] = '\0'; newSprintGoal[0] = '\0';
    }
    ImGui::EndChild();
    ImGui::NextColumn();

    ImGui::BeginChild("SprintDetails");
    Sprint* selectedSprint = project.GetSprintById(planning_selectedSprintId);
    if (selectedSprint) {
        ImGui::Text("Details for: %s", selectedSprint->name.c_str());
        ImGui::Text("Goal: %s", selectedSprint->goal.c_str());
        ImGui::Text("Status: %s", SprintStatusToString(selectedSprint->status));
        if (selectedSprint->status == SprintStatus::PLANNING && ImGui::Button("Activate Sprint")) {
             Sprint* active = project.GetActiveSprint();
             if (active) active->status = SprintStatus::COMPLETED;
             selectedSprint->status = SprintStatus::ACTIVE;
        }
        ImGui::SameLine();
        if (selectedSprint->status == SprintStatus::ACTIVE && ImGui::Button("Complete Sprint")) {
            selectedSprint->status = SprintStatus::COMPLETED;
        }
        ImGui::Separator();

        if (selectedSprint->status == SprintStatus::PLANNING) {
            ImGui::Columns(2, "planning_columns", false);
            ImGui::Text("Add Stories from Backlog");
            ImGui::BeginChild("BacklogStories", ImVec2(0,0), true);
            for (auto* story : project.GetUnassignedStories()) {
                 ImGui::BulletText("%s", story->title.c_str());
                 ImGui::SameLine();
                 if (ImGui::SmallButton(("[+]##" + std::to_string(story->id)).c_str())) {
                     project.AssignStoryToSprint(story->id, selectedSprint->id);
                 }
            }
            ImGui::EndChild();
            ImGui::NextColumn();
            ImGui::Text("Stories in this Sprint");
            ImGui::BeginChild("SprintStories", ImVec2(0,0), true);
            for (size_t i = 0; i < selectedSprint->storyIds.size(); ++i) {
                UserStory* story = project.GetStoryById(selectedSprint->storyIds[i]);
                if (story) {
                    ImGui::BulletText("%s", story->title.c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton(("[X]##" + std::to_string(story->id)).c_str())) {
                        project.RemoveStoryFromSprint(story->id, selectedSprint->id);
                        break;
                    }
                }
            }
            ImGui::EndChild();
            ImGui::Columns(1);
        } else {
            ImGui::Text("Stories in this Sprint");
            ImGui::BeginChild("SprintStories", ImVec2(0,0), true);
            for (int storyId : selectedSprint->storyIds) {
                 UserStory* story = project.GetStoryById(storyId);
                 if(story) ImGui::BulletText("%s", story->title.c_str());
            }
            ImGui::EndChild();
        }
    } else {
        ImGui::Text("No sprint selected.");
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Please select a sprint from the left panel to see its details, or create a new one.");
    }
    ImGui::EndChild();
    ImGui::Columns(1);
}

// User Management UI
void UIManager::ShowUsersWindow(AuthenticationManager& auth) {
    if(headerFont) ImGui::PushFont(headerFont);
    ImGui::Text("User Management");
    if(headerFont) ImGui::PopFont();
    ImGui::Separator();

    ImGui::InputText("Username", newUsername, sizeof(newUsername));
    ImGui::InputText("Password", newPassword, sizeof(newPassword), ImGuiInputTextFlags_Password);
    ImGui::InputText("Full Name", newFullName, sizeof(newFullName));
    const char* roles[] = { "Developer", "Scrum Master", "Product Owner", "Tester" };
    ImGui::Combo("Role", &newUserRole, roles, IM_ARRAYSIZE(roles));
    if (ImGui::Button("[+] Add User")) {
        auto status = auth.CreateUser(newUsername, newPassword, newFullName, static_cast<UserRole>(newUserRole));
        if (status == AuthenticationManager::CreateUserStatus::SUCCESS) {
            userCreationError = "";
            newUsername[0] = '\0'; newPassword[0] = '\0'; newFullName[0] = '\0';
        } else if (status == AuthenticationManager::CreateUserStatus::USERNAME_EXISTS) {
            userCreationError = "Error: This username is already taken.";
        }
    }

    if (!userCreationError.empty()) {
        ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "%s", userCreationError.c_str());
    }

    ImGui::Separator();
    ImGui::Text("Existing Users:");
    for (const auto& user : auth.users) {
        ImGui::Text("- %s (%s) [%s]", user.username.c_str(), user.fullName.c_str(), RoleToString(user.role));
    }
}

// Show Create Story and Task Popups
void UIManager::ShowCreateStoryPopup(AuthenticationManager& auth, ProjectManager& project) {
    ImGui::OpenPopup("Add New Story");
    if (ImGui::BeginPopupModal("Add New Story", &openCreateStoryPopup, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::InputText("Title", newStoryTitle, sizeof(newStoryTitle));
        ImGui::InputTextMultiline("Description", newStoryDesc, sizeof(newStoryDesc));
        ImGui::Spacing();
        if (ImGui::Button("Create", ImVec2(120, 0))) {
            project.CreateUserStory(newStoryTitle, newStoryDesc, auth.GetCurrentUser()->username);
            newStoryTitle[0] = '\0'; newStoryDesc[0] = '\0';
            openCreateStoryPopup = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            newStoryTitle[0] = '\0'; newStoryDesc[0] = '\0';
            openCreateStoryPopup = false;
        }
        ImGui::EndPopup();
    }
}

void UIManager::ShowCreateTaskPopup(AuthenticationManager& auth, ProjectManager& project) {
    ImGui::OpenPopup("Add New Task");
    if (ImGui::BeginPopupModal("Add New Task", &openCreateTaskPopup, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::InputText("Task Title", newTaskTitle, sizeof(newTaskTitle));
        ImGui::InputTextMultiline("Description", newTaskDesc, sizeof(newTaskDesc));
        ImGui::InputInt("Estimated Hours", &newTaskHours);
        ImGui::InputText("Deadline (YYYY-MM-DD)", newTaskDeadline, sizeof(newTaskDeadline));
        std::vector<const char*> usernames;
        for (const auto& user : auth.users) {
            if (user.role == UserRole::DEVELOPER || user.role == UserRole::TESTER) {
                usernames.push_back(user.username.c_str());
            }
        }
        ImGui::Combo("Assign to", &assignedUserIndex, usernames.data(), usernames.size());
        ImGui::Spacing();
        if (ImGui::Button("Create Task", ImVec2(120, 0))) {
            std::string assigned_user = (assignedUserIndex >= 0 && assignedUserIndex < usernames.size()) ? usernames[assignedUserIndex] : "";
            project.CreateTask(storyIdForNewTask, newTaskTitle, newTaskDesc, auth.GetCurrentUser()->username, assigned_user, newTaskHours, newTaskDeadline);
            storyIdForNewTask = -1;
            newTaskTitle[0] = '\0'; newTaskDesc[0] = '\0'; newTaskDeadline[0] = '\0';
            newTaskHours = 1; assignedUserIndex = -1;
            openCreateTaskPopup = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            storyIdForNewTask = -1;
            newTaskTitle[0] = '\0'; newTaskDesc[0] = '\0'; newTaskDeadline[0] = '\0';
            newTaskHours = 1; assignedUserIndex = -1;
            openCreateTaskPopup = false;
        }
        ImGui::EndPopup();
    }
}

// Show Task Details Popup
void UIManager::ShowTaskDetailsPopup(AuthenticationManager& auth, ProjectManager& project) {
    Task* selectedTask = project.GetTaskById(details_selectedTaskId);
    if (!selectedTask) return;

    ImGui::OpenPopup("Task Details");
    static char deadlineBuffer[64];
    if (ImGui::IsPopupOpen("Task Details")) {
        strncpy(deadlineBuffer, selectedTask->deadline.c_str(), sizeof(deadlineBuffer));
    }

    if (ImGui::BeginPopupModal("Task Details", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        if(headerFont) ImGui::PushFont(headerFont);
        ImGui::Text("%s", selectedTask->title.c_str());
        if(headerFont) ImGui::PopFont();
        ImGui::Separator();

        ImGui::Text("Progress (%d / %d hours)", selectedTask->loggedHours, selectedTask->estimatedHours);
        float progress = (selectedTask->estimatedHours > 0) ? ((float)selectedTask->loggedHours / selectedTask->estimatedHours) : 0.0f;
        char progress_text[32];
        snprintf(progress_text, sizeof(progress_text), "%d%%", (int)(progress * 100));
        ImGui::ProgressBar(progress, ImVec2(-1, 0), progress_text);
        ImGui::Separator();

        int current_status = static_cast<int>(selectedTask->status);
        const char* statuses[] = {"To Do", "In Progress", "Testing", "Done"};
        if (ImGui::Combo("Status", &current_status, statuses, IM_ARRAYSIZE(statuses))) {
            selectedTask->status = static_cast<TaskStatus>(current_status);
        }
        if (ImGui::InputText("Deadline", deadlineBuffer, sizeof(deadlineBuffer))) {
            selectedTask->deadline = deadlineBuffer;
        }

        if (auth.CanPerformAction(UserRole::SCRUM_MASTER) || auth.CanPerformAction(UserRole::PRODUCT_OWNER)) {
            std::vector<const char*> usernames;
            std::vector<std::string> username_strs;
            int current_assignee_idx = 0;

            username_strs.push_back("Unassigned");

            for(const auto& user : auth.users) {
                if(user.role == UserRole::DEVELOPER || user.role == UserRole::TESTER) {
                    username_strs.push_back(user.username);
                }
            }
            for(const auto& name : username_strs) usernames.push_back(name.c_str());

            for(size_t i = 0; i < usernames.size(); ++i) {
                if (selectedTask->assignedTo == usernames[i]) {
                    current_assignee_idx = i;
                    break;
                }
            }

            if (ImGui::Combo("Assigned To", &current_assignee_idx, usernames.data(), usernames.size())) {
                if (std::string(usernames[current_assignee_idx]) == "Unassigned") {
                    selectedTask->assignedTo = "";
                } else {
                    selectedTask->assignedTo = usernames[current_assignee_idx];
                }
            }
        } else {
            if (ImGui::Button("Assign to me")) {
                selectedTask->assignedTo = auth.GetCurrentUser()->username;
            }
            ImGui::SameLine();
            ImGui::Text("Assigned to: %s", selectedTask->assignedTo.c_str());
        }

        ImGui::Separator();
        ImGui::Text("Log Work:");
        static int hours = 1;
        static char comment[128] = "";
        ImGui::InputInt("Hours", &hours);
        ImGui::InputText("Comment", comment, sizeof(comment));
        if (ImGui::Button("Log Work")) {
            project.LogWork(selectedTask->id, auth.GetCurrentUser()->username, hours, comment);
            hours = 1; comment[0] = '\0';
        }
        ImGui::Separator();
        ImGui::Text("Work History:");
        ImGui::BeginChild("WorkLog", ImVec2(0, 100), true);
        for(const auto& log : selectedTask->workLog) {
            ImGui::TextWrapped("%s", log.c_str());
        }
        ImGui::EndChild();
        if (ImGui::Button("Close", ImVec2(-1, 0))) {
            details_selectedTaskId = -1;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    } else {
        details_selectedTaskId = -1;
    }
}