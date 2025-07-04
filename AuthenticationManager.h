#pragma once
#include "DataTypes.h"
#include <vector>
#include <string>

/**
 * @class AuthenticationManager
 * @brief Manages user authentication, sessions, and permissions.
 *
 * This class handles logging in, logging out, creating new users,
 * and checking if the logged-in user has the required permissions for an action.
 */
class AuthenticationManager {
public:
    /**
     * @enum CreateUserStatus
     * @brief Represents the result of a create user operation.
     */
    enum class CreateUserStatus {
        SUCCESS,            ///< The user was created successfully.
        USERNAME_EXISTS,    ///< A user with that username already exists.
        PERMISSION_DENIED   ///< The current user does not have permission to create users.
    };

    /**
     * @brief Attempts to log in a user.
     * @param username The username to log in with.
     * @param password The password for the user.
     * @return True if the login was successful, false otherwise.
     */
    bool Login(const std::string& username, const std::string& password);

    /**
     * @brief Logs out the current user.
     */
    void Logout();

    /**
     * @brief Checks if a user is currently logged in.
     * @return True if a user is logged in.
     */
    bool IsLoggedIn() const;

    /**
     * @brief Gets a pointer to the currently logged-in user object.
     * @return A const pointer to the User object, or nullptr if no one is logged in.
     */
    const User* GetCurrentUser() const;

    /**
     * @brief Creates a new user in the system.
     * @param username The new user's username.
     * @param password The new user's password.
     * @param fullName The new user's full name.
     * @param role The role for the new user.
     * @return A CreateUserStatus indicating the result of the operation.
     */
    CreateUserStatus CreateUser(const std::string& username, const std::string& password, const std::string& fullName, UserRole role);

    /**
     * @brief Checks if the current user can perform an action requiring a specific role.
     * @param requiredRole The minimum role required for the action.
     * @return True if the user has sufficient permissions.
     */
    bool CanPerformAction(UserRole requiredRole) const;

    /**
     * @brief Checks if the current user can manage sprints (Scrum Master or higher).
     * @return True if they have permission.
     */
    bool CanManageSprints() const;

    /**
     * @brief Checks if the current user can manage other users (Scrum Master only).
     * @return True if they have permission.
     */
    bool CanManageUsers() const;

    /// @brief A vector containing all users in the system.
    std::vector<User> users;

private:
    /// @brief The username of the currently logged-in user. Empty if no one is logged in.
    std::string currentUsername;
};