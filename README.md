<div align="center">
<img src="https://capsule-render.vercel.app/api?type=waving&color=gradient&height=250&section=header&text=Scrum%20App&fontSize=80&fontColor=ffffff" alt="Header Banner"/>
</div>

<div align="center">

</div>

<p align="center">
A desktop Scrum project management application built with C++ and the <b>Dear ImGui</b> graphical user interface library. This tool provides a comprehensive suite of features for managing users, sprints, user stories, and tasks, following core Agile principles.
</p>

📋 Table of Contents

🌟 Core Features

🖼️ Screenshots

🛠️ Technology Stack

🚀 Getting Started

📂 Project Architecture

🤝 Contributing

📄 License

🌟 Core Features
✅ Secure User Authentication: A robust login system with distinct roles (Developer, Tester, Product Owner, Scrum Master).

✅ Role-Based Permissions: The UI and available actions are dynamically adjusted based on the logged-in user's role.

✅ Product Backlog Management: Easily create, view, and prioritize user stories in the product backlog.

✅ Sprint Planning & Management: Organize work into sprints, assign user stories, and track sprint status.

✅ Interactive Task Board: A drag-and-drop Kanban-style board to visualize task progress.

✅ Detailed Task Management: Create tasks, assign them, set deadlines, and log work hours with comments.

✅ Insightful Dashboard: View the active sprint's burndown chart and a summary of your assigned tasks.

✅ Data Persistence: All project data is saved to local text files, ensuring your work is preserved between sessions.

🖼️ Screenshots

<div align="center">
<img src="https://imgur.com/a/1juVICV" alt="Scrum App Screenshot"/>
</div>

🛠️ Technology Stack
Core Language: C++17

GUI Framework: Dear ImGui

Windowing & Rendering: SDL2 & OpenGL

Build System: CMake

🚀 Getting Started
To get a local copy up and running, follow these simple steps.

Prerequisites

You will need a C++ compiler and the following tools/libraries installed on your system:

CMake (version 3.10 or higher)

SDL2

Dear ImGui library: The project requires the Dear ImGui library files to be present in the project directory to compile.

Building the Project

Clone the repository:

git clone https://github.com/oskarbednarek/Scrum-App.git
cd ScrumApp

Create a build directory:

mkdir build
cd build

Run CMake and build the project:

cmake ..
make

(On Windows with Visual Studio, you may need to use cmake --build .)

Run the application:
The executable ScrumApp will be located in the build/ directory.

📂 Project Architecture
<details>
<summary>Click to expand the project structure overview</summary>

The project is organized with a clear separation of concerns, with each major component handled by a dedicated manager class:

ScrumApp: The main application class that initializes SDL and ImGui, and runs the main event loop.

UIManager: Responsible for rendering the entire user interface using ImGui. It is decoupled from the core application logic.

ProjectManager: Manages the lifecycle of user stories, tasks, and sprints.

AuthenticationManager: Handles user login, session management, and role-based permissions.

DataManager: Encapsulates all file I/O logic for saving and loading project data.

DataTypes.h: A central header defining all core data structures (User, UserStory, Task, Sprint) and enums.

</details>

🤝 Contributing
Contributions, issues, and feature requests are welcome! Feel free to check the issues page for this repository.

📄 License
This project is licensed under the MIT License. See the LICENSE file for more information.

