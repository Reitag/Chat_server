#include "chat.h"

Chat::Chat()
{

}

Chat::~Chat()
{
	users.clear();
	users.shrink_to_fit();
}

bool Chat::isEqualName(const User& currentUser, std::string& userName)
{
	if (currentUser.GetUserNameW().compare(userName) == 0)
	{
		return true;
	}
	return false;
}

bool Chat::isEqualLogin(const User& currentUser, std::string& login)
{
	if (currentUser.GetLogin().compare(login) == 0) // .comapre() возвращает 0, если они равны!
	{
		return true;
	}
	return false;
}

bool Chat::isEqualPass(const User& currentUser, uint*& sha1_password)
{
	if (*currentUser.GetSha1Pass() == *sha1_password)
	{
		return true;
	}
	return false;
}

void Chat::addUser(const User& newUser)
{
	users.push_back(newUser);
}

void Chat::addMessage(User& currentUser, std::string& message)
{
	currentUser._messages.push_back(message);
}

void Chat::SetNewPass(User& currentUser, uint*& newPassword)
{
	currentUser._authData.second = newPassword;
}

void Chat::printAllMessages(const User& currentUser, std::vector<std::string>& messages)
{
	for (std::string msg : currentUser._messages)
	{
		messages.push_back(msg);
	}
}

void Chat::registration(std::string userName, std::string login, std::string password)
{
	User newUser;
	newUser._userName = userName;
	uint* sha1_password = sha1(const_cast<char*>(password.c_str()), password.length());
	newUser._authData = make_pair(login, sha1_password); // login is 'key' , sha1_password is 'value'
	addUser(newUser);
}

void Chat::sendMessage(std::string login, std::string message)
{
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (isEqualLogin(*it, login) == true)
		{
			addMessage(*it, message);
			break;
		}
	}
}

bool Chat::checkUser(std::string login, std::string password)
{
	uint* sha1_password = sha1(const_cast<char*>(password.c_str()), password.length());
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (isEqualLogin(*it, login) == true && isEqualPass(*it, sha1_password) == true)
		{
			return true;
		}
	}
	return false;
}

bool Chat::checkUserByName(std::string& userName)
{
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (isEqualName(*it, userName) == true)
		{
			return true;
		}
	}
	return false;
}

bool Chat::checkPassByName(std::string& userName, std::string& password)
{
	uint* sha1_password = sha1(const_cast<char*>(password.c_str()), password.length());
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (isEqualPass(*it, sha1_password) == true)
		{
			return true;
		}
	}
	return false;
}

bool Chat::checkLoginExist(std::string& login)
{
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (isEqualLogin(*it, login))
		{
			return true;
		}
	}
	return false;
}

std::string Chat::GetLoginByUserName(std::string userName)
{
	User currentUser;
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		currentUser = *it;
		if ((currentUser.GetUserNameW().compare(userName) == 0))
		{
			return currentUser.GetLogin();
		}
	}
}

std::string Chat::GetNameByLogin(std::string login)
{
	User currentUser;
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		currentUser = *it;
		if (currentUser.GetLogin().compare(login) == 0)
		{
			return currentUser.GetUserNameW();
		}
	}
}

void Chat::SetNewPassByName(std::string& userName, std::string& newPassword)
{
	uint* sha1_newPassword = sha1(const_cast<char*>(newPassword.c_str()), newPassword.length());
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (isEqualName(*it, userName) == true)
		{
			SetNewPass(*it, sha1_newPassword);
		}
	}
}

void Chat::sendMessageToAllUsersExceptCurrentUser(std::string login, std::string& message)
{
	User currentUser;
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (isEqualLogin(*it, login) == true)
		{
			continue;
		}
		else
		{
			currentUser = *it;
			sendMessage(currentUser.GetLogin(), message);
		}
	}
}

void Chat::printAllUsersExceptCurrentUser(std::string login, std::vector<std::string>& vecStr)
{
	User currentUser;
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (isEqualLogin(*it, login) == true)
		{
			continue;
		}
		else
		{
			currentUser = *it;
			vecStr.push_back("Логин: " + currentUser.GetLogin() + " (Имя пользователя: " + currentUser.GetUserNameW() + ")");
		}
	}
}

void Chat::showAllUsers(std::vector<std::string>& vecStr)
{
	if (!users.empty())
	{
		User currentUser;
		for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
		{
			currentUser = *it;
			vecStr.push_back("-------\nИмя: " + currentUser.GetUserNameW() + "\nЛогин: " + currentUser.GetLogin());
		}
	}
	else
	{
		vecStr.push_back("Не создано ещё ниодного аккаунта");
	}
}


void Chat::print(std::string login, std::vector<std::string>& messages)
{
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (isEqualLogin(*it, login) == true)
		{
			printAllMessages(*it, messages);
			break;
		}
	}
}

void clearCin() // Очищает буфер от символов, введённых через cin
{
	std::cin.clear();
	std::cin.ignore(32767, '\n');
}