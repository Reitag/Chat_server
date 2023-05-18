#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include "sha_1.h" // string.h included in sha_1.h

class Chat
{
private:
	struct User
	{
		std::string _userName;
		std::pair<std::string, uint*> _authData;
		std::list<std::string> _messages;

		User() : _userName("")
		{

		}
		~User()
		{

		}
		std::string GetLogin() const
		{
			return _authData.first;
		}
		std::string GetUserName() const
		{
			return _userName;
		}
		uint* GetSha1Pass() const
		{
			return _authData.second;
		}
	};
	std::vector<User> users;
	bool isEqualName(const User& currentUser, std::string& userName);
	bool isEqualLogin(const User& currentUser, std::string& login);
	bool isEqualPass(const User& currentUser, uint*& sha1_password);
	void addUser(const User& newUser);
	void addMessage(User& currentUser, std::string& message);
	void SetNewPass(User& currentUser, uint*& newPassword);
	void printAllMessages(const User& currentUser, std::vector<std::string>& messages);

public:
	Chat();
	~Chat();
	void registration(std::string userName, std::string login, std::string password);
	void sendMessage(std::string login, std::string message);
	bool checkUser(std::string login, std::string password);
	bool checkUserByName(std::string& userName);
	bool checkPassByName(std::string& userName, std::string& password);
	bool checkLoginExist(std::string& login);
	std::string GetLoginByUserName(std::string userName);
	std::string GetNameByLogin(std::string login);
	void SetNewPassByName(std::string& userName, std::string& newPassword);
	void sendMessageToAllUsersExceptCurrentUser(std::string login, std::string& message);
	void printAllUsersExceptCurrentUser(std::string login, std::vector<std::string>& vecStr);
	void showAllUsers(std::vector<std::string>& vecStr);
	void print(std::string login, std::vector<std::string>& messages);
};
void clearCin();
