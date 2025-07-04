Scrum App - C++ Management Tool
<div align="center">

</div>

A desktop Scrum project management application built with C++ and the Dear ImGui graphical user interface library. This tool provides a comprehensive suite of features for managing users, sprints, user stories, and tasks, following core Agile principles.

Replace this with a real screenshot of your application's UI.

🌟 Core Features
Secure User Authentication: A robust login system with distinct roles (Developer, Tester, Product Owner, Scrum Master).

Role-Based Permissions: The UI and available actions are dynamically adjusted based on the logged-in user's role, ensuring data integrity and proper workflow.

Product Backlog Management: Easily create, view, and prioritize user stories in the product backlog.

Sprint Planning & Management: Organize work into sprints, assign user stories from the backlog, and track sprint status from planning to completion.

Interactive Task Board: A drag-and-drop Kanban-style board to visualize task progress through "To Do", "In Progress", "Testing", and "Done" columns.

Detailed Task Management: Create tasks within user stories, assign them to team members, set deadlines, and log work hours with comments.

Insightful Dashboard: View the active sprint's burndown chart and a personalized summary of your currently assigned tasks.

Data Persistence: All project data (users, stories, tasks, sprints) is saved to local text files, ensuring your work is preserved between sessions.

🛠️ Technology Stack
Core Language: C++17

GUI Framework: Dear ImGui

Windowing & Rendering: SDL2 and OpenGL

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

git clone https://github.com/your-username/ScrumApp.git
cd ScrumApp


Create a build directory:

mkdir build
cd build


Run CMake and build the project:

cmake ..
make 


(On Windows with Visual Studio, you may need to use cmake --build . instead of make)

Run the application:
The executable ScrumApp will be located in the build directory.

📂 Project Architecture
The project is organized with a clear separation of concerns, with each major component handled by a dedicated manager class:

ScrumApp: The main application class that initializes SDL and ImGui, and runs the main event loop.

UIManager: Responsible for rendering the entire user interface using ImGui. It is decoupled from the core application logic.

ProjectManager: Manages the lifecycle of user stories, tasks, and sprints.

AuthenticationManager: Handles user login, session management, and role-based permissions.

DataManager: Encapsulates all file I/O logic for saving and loading project data.

DataTypes.h: A central header defining all core data structures (User, UserStory, Task, Sprint) and enums.

🤝 Contributing
Contributions, issues, and feature requests are welcome! Feel free to check the issues page for this repository.

📄 License
This project is licensed under the MIT License. See the LICENSE file for more information.
