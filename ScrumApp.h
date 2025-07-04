#pragma once
#include "DataManager.h"
#include "AuthenticationManager.h"
#include "ProjectManager.h"
#include "UIManager.h"
#include <SDL.h>
#include <memory>

class ScrumApp {
public:
 ScrumApp();
 ~ScrumApp();

 bool Initialize();
 void Run();
 void Shutdown();

private:
 void LoadData();
 void SaveData();

 SDL_Window* window = nullptr;
 SDL_GLContext gl_context = nullptr;
 bool isRunning = false;

 DataManager dataManager;
 AuthenticationManager authManager;
 ProjectManager projectManager;

 std::unique_ptr<UIManager> uiManager;
};