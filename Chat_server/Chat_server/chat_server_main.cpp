#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include "chat.h"

int main()
{
	SetConsoleCP(1251); // Возможность читать и вносить через cin кирилицу
	SetConsoleOutputCP(1251);

	WSAData wsData; // Инициализация WinSock
	int checkStatus = WSAStartup(MAKEWORD(2, 2), &wsData);
	if (checkStatus != 0)
	{
		std::cout << "Error WinSock version initializaion #" << WSAGetLastError() << std::endl;
		return 1;
	}
	else
	{
		std::cout << "WinSock successfully initialized" << std::endl;
	}

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0); // Создание server socket
	if (serverSocket == INVALID_SOCKET)
	{
		std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		std::cout << "Server socket successfully initialized" << std::endl;
	}

	in_addr ip_to_num; // Перевод IP в поток байтов
	checkStatus = inet_pton(AF_INET, "127.0.0.1", &ip_to_num);
	if (checkStatus <= 0)
	{
		std::cout << "Error in IP translation to special numeric format" << std::endl;
		return 1;
	}

	sockaddr_in serverInfo; // Создание структуры сокета для сервера
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr = ip_to_num;
	serverInfo.sin_port = htons(55555);

	checkStatus = bind(serverSocket, (sockaddr*)&serverInfo, sizeof(serverInfo)); // bind
	if (checkStatus != 0)
	{
		std::cout << "Error Socket binding to server info. Error # " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		std::cout << "Binding socket successfully completed" << std::endl;
	}

	checkStatus = listen(serverSocket, SOMAXCONN); // Прослушка
	if (checkStatus != 0) {
		std::cout << "Can't start to listen to. Error # " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		std::cout << "Listening..." << std::endl;
	}

	sockaddr_in clientInfo; // Создание структуры клиента
	ZeroMemory(&clientInfo, sizeof(clientInfo));
	int client_size = sizeof(clientInfo);
	SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientInfo, &client_size); // Создание сокета для клиента функцией accept
	if (clientSocket == INVALID_SOCKET)
	{
		std::cout << "Client detected, but can't connect to a client. Error # " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		std::cout << "Connection to a client established successfully" << std::endl;
	}

	Chat chat;
	std::string userName, login, password, message;
	short count = 0, packet_size = 0;
	std::vector <char> buffer(1024);
	char open;

	packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL);
	open = buffer[0];

	while (open != '4')
	{
		switch (open)
		{
		case '1':
		{
			std::cout << "Case 1 has been opened" << std::endl;
			packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL);
			userName.clear();
			for (int i = 0; i < 1024; ++i)
			{
				if (buffer[i] == '\n')
				{
					break;
				}
				else
				{
					userName += buffer[i];
				}
			}
			std::cout << userName << std::endl;
			packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL);
			login.clear();
			for (int i = 0; i < 1024; ++i)
			{
				if (buffer[i] == '\n')
				{
					break;
				}
				else
				{
					login += buffer[i];
				}
			}
			while (chat.checkLoginExist(login) == true)
			{
				buffer[0] = '1';
				packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
				packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL);
				login.clear();
				for (int i = 0; i < 1024; ++i)
				{
					if (buffer[i] == '\n')
					{
						break;
					}
					else
					{
						login += buffer[i];
					}
				}
			}
			buffer[0] = '0';
			packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
			std::cout << login << std::endl;
			packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL);
			password.clear();
			for (int i = 0; i < 1024; ++i)
			{
				if (buffer[i] == '\n')
				{
					break;
				}
				else
				{
					password += buffer[i];
				}
			}
			std::cout << password << std::endl;

			chat.registration(userName, login, password);
			std::cout << "User has been added to database successfully." << std::endl;
			++count;
			userName.clear();
			login.clear();
			password.clear();

			break;
		}
		case '2':
		{
			std::cout << "Case 2 has been opened" << std::endl;
			packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL);
			login.clear();
			for (int i = 0; i < 1024; ++i)
			{
				if (buffer[i] == '\n')
				{
					break;
				}
				else
				{
					login += buffer[i];
				}
			}
			packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL);
			password.clear();
			for (int i = 0; i < 1024; ++i)
			{
				if (buffer[i] == '\n')
				{
					break;
				}
				else
				{
					password += buffer[i];
				}
			}
			if (chat.checkUser(login, password) == true)
			{
				buffer[0] = '0';
				packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
				
				std::cout << "User " << login << " has entered on his account." << std::endl;

				std::string nameOfCurrentUser = chat.GetNameByLogin(login);

				char choiceInsideCase2;
				packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL);
				choiceInsideCase2 = buffer[0];

				while (choiceInsideCase2 != '4')
				{
					switch (choiceInsideCase2)
					{
					case '1':
					{
						char choiceInsideCase2InsideCase1;
						packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL);
						choiceInsideCase2InsideCase1 = buffer[0];

						while (choiceInsideCase2InsideCase1 != '3')
						{
							switch (choiceInsideCase2InsideCase1)
							{
							case '1': // сообщение пользователю
							{
								std::vector<std::string> vecStr;
								std::string str;
								chat.printAllUsersExceptCurrentUser(chat.GetLoginByUserName(nameOfCurrentUser), vecStr);
								buffer.clear();
								buffer.resize(1024);
								for (int i = 0; i < vecStr.size(); ++i)
								{
									str = vecStr[i];
									for (int j = 0; j < str.size(); ++j)
									{
										buffer[j] = str[j];
									}
									packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
									str.clear();
									buffer.clear();
									buffer.resize(1024);
								}
								vecStr.clear();
								str.clear();
								buffer.clear();
								buffer.resize(1024);
								packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL);
								login.clear();
								for (int i = 0; i < 1024; ++i)
								{
									if (buffer[i] == '\n')
									{
										break;
									}
									else
									{
										login += buffer[i];
									}
								}
								if (chat.checkLoginExist(login) == true)
								{
									buffer[0] = '0';
									packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
									packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL);
									message.clear();
									for (int i = 0; i < 1024; ++i)
									{
										if (buffer[i] == '\0')
										{
											break;
										}
										else
										{
											message += buffer[i];
										}
									}
									message = "У вас сообщение от пользователя " + nameOfCurrentUser + " " + "(" + chat.GetLoginByUserName(nameOfCurrentUser) + ")" + ": " + '\"' + message + '\"';
									chat.sendMessage(login, message);
									std::cout << "Message was send successfully" << std::endl;
								}
								else
								{
									buffer[0] = '1';
									packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
								}

								break;
							}
							case '2': // групповой чат
							{
								std::cout << "Group chat still does not exist. There is only sending messages between server and one client." << std::endl;
								while (true)
								{
									std::string currentUser = chat.GetNameByLogin(login);;
									packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL);
									if (buffer[0] == 'q')
									{
										break;
									}
									for (int i = 0; i < 1024; ++i)
									{
										if (buffer[i] == '\n')
										{
											break;
										}
										else
										{
											message += buffer[i];
										}
									}
									std::cout << currentUser << ": " << message << std::endl;
									message.clear();
									fgets(buffer.data(), buffer.size(), stdin);
									packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
								}

								break;
							}
							default:
							{
								std::cout << "Неверное значение\n" << std::endl;

								break;
							}
							}
							std::cout << "choiceInsideCase2InsideCase1 is waiting" << std::endl;
							packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL);
							choiceInsideCase2InsideCase1 = buffer[0];
							std::cout << "choiceInsideCase2InsideCase1 " << choiceInsideCase2InsideCase1 << std::endl;
						};

						break;
					}
					case '2': // показать сообщения
					{
						std::cout << "Showing messages is opened" << std::endl;
						std::vector<std::string> messages;
						std::string str;
						chat.print(login, messages);
						buffer[0] = messages.size();
						packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
						for (int i = 0; i < messages.size(); ++i)
						{
							str = messages[i];
							for (int j = 0; j < str.size(); ++j)
							{
								buffer[j] = str[j];
							}
							packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
							str.clear();
							buffer.clear();
							buffer.resize(1024);
						}
						messages.clear();
						str.clear();
						buffer.clear();
						buffer.resize(1024);

						break;
					}
					case '3': //Смена пароля на новый
					{
						std::cout << "User " << nameOfCurrentUser << " wants to change his password." << std::endl;
						packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL);
						password.clear();
						for (int i = 0; i < 1024; ++i)
						{
							if (buffer[i] == '\n')
							{
								break;
							}
							else
							{
								password += buffer[i];
							}
						}
						while (chat.checkPassByName(nameOfCurrentUser, password) == false) // проверка на правильность старого пароля
						{
							buffer[0] = '1';
							packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
							packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL);
							password.clear();
							for (int i = 0; i < 1024; ++i)
							{
								if (buffer[i] == '\n')
								{
									break;
								}
								else
								{
									password += buffer[i];
								}
							}
						}
						buffer[0] = '0';
						packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
						packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL);
						password.clear();
						for (int i = 0; i < 1024; ++i)
						{
							if (buffer[i] == '\n')
							{
								break;
							}
							else
							{
								password += buffer[i];
							}
						}
						while (chat.checkPassByName(nameOfCurrentUser, password) == true) // проверка, что новый пароль такой же как старый
						{
							buffer[0] = '1';
							packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
							packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL);
							password.clear();
							for (int i = 0; i < 1024; ++i)
							{
								if (buffer[i] == '\n')
								{
									break;
								}
								else
								{
									password += buffer[i];
								}
							}
						}
						buffer[0] = '0';
						packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
						chat.SetNewPassByName(nameOfCurrentUser, password);

						break;
					}
					default:
					{

						break;
					}
					}
					std::cout << "choiceInsideCase2 is waiting" << std::endl;
					packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL);
					choiceInsideCase2 = buffer[0];
					std::cout << "choiceInsideCase2 " << choiceInsideCase2 << std::endl;
				};

				break;
			}
			else
			{
				buffer[0] = '1';
				packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);

				break;
			}
			break;
		}
		case '3':
		{
			std::vector<std::string> vecStr;
			std::string str;
			chat.showAllUsers(vecStr);
			for (int i = 0; i < vecStr.size(); ++i)
			{
				str = vecStr[i];
				for (int j = 0; j < str.size(); ++j)
				{
					buffer[j] = str[j];
				}
				packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
				str.clear();
				buffer.clear();
				buffer.resize(1024);
			}
			vecStr.clear();
			str.clear();
			buffer.clear();
			buffer.resize(1024);

			break;
		}
		default:
		{
			std::cout << "Неверное значение\n" << std::endl;

			break;
		};
		}
		std::cout << "Open is waiting" << std::endl;
		packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL);
		open = buffer[0];
		std::cout << "Open is " << open << std::endl;
	}

	closesocket(serverSocket);
	closesocket(clientSocket);
	WSACleanup();
	return 0;
}