#include "AuthenticationManager.h"
#include <algorithm>

bool AuthenticationManager::Login(const std::string& username, const std::string& password) {
    auto it = std::find_if(users.begin(), users.end(), [&](const User& user) {
        return user.username == username && user.password == password;
    });
    if (it != users.end()) {
        currentUsername = username;
        return true;
    }
    return false;
}

void AuthenticationManager::Logout() {
    currentUsername.clear();
}

bool AuthenticationManager::IsLoggedIn() const {
    return !currentUsername.empty();
}

const User* AuthenticationManager::GetCurrentUser() const {
    if (!IsLoggedIn()) {
        return nullptr;
    }
    auto it = std::find_if(users.begin(), users.end(), [&](const User& user) {
        return user.username == currentUsername;
    });
    if (it != users.end()) {
        return &(*it);
    }
    return nullptr;
}



AuthenticationManager::CreateUserStatus AuthenticationManager::CreateUser(const std::string& username, const std::string& password, const std::string& fullName, UserRole role) {
    if (!CanManageUsers()) {
        return CreateUserStatus::PERMISSION_DENIED;
    }

    auto it = std::find_if(users.begin(), users.end(), [&](const User& user) {
        return user.username == username;
    });

    if (it != users.end()) {
        return CreateUserStatus::USERNAME_EXISTS; // User already exists
    }

    users.push_back({username, password, fullName, role});
    return CreateUserStatus::SUCCESS;
}


bool AuthenticationManager::CanPerformAction(UserRole requiredRole) const {
    const User* currentUser = GetCurrentUser();
    if (!currentUser) return false;

    if (currentUser->role == UserRole::SCRUM_MASTER) return true;
    return currentUser->role == requiredRole;
}

bool AuthenticationManager::CanManageSprints() const {
    const User* currentUser = GetCurrentUser();
    if (!currentUser) return false;
    return currentUser->role == UserRole::SCRUM_MASTER || currentUser->role == UserRole::PRODUCT_OWNER;
}

bool AuthenticationManager::CanManageUsers() const {
    const User* currentUser = GetCurrentUser();
    if (!currentUser) return false;
    return currentUser->role == UserRole::SCRUM_MASTER;
}