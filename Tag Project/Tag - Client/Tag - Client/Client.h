#pragma once
#ifdef _DEBUG  
#pragma comment(lib,"sfml-graphics-d.lib")  
#pragma comment(lib,"sfml-audio-d.lib")  
#pragma comment(lib,"sfml-system-d.lib")  
#pragma comment(lib,"sfml-window-d.lib")  
#pragma comment(lib,"sfml-network-d.lib")  
#else  
#pragma comment(lib,"sfml-graphics.lib")  
#pragma comment(lib,"sfml-audio.lib")  
#pragma comment(lib,"sfml-system.lib")  
#pragma comment(lib,"sfml-window.lib")  
#pragma comment(lib,"sfml-network.lib")  
#endif  
#include <iostream>
#include <SFML/Graphics.hpp> 
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib") //Required for WinSock
#include <WinSock2.h> //For win sockets
#include <string> //For std::string
#include <iostream> //For std::cout, std::endl, std::cin.getline
enum Packet
{
	P_Message
};

class Client
{
public: //Public functions
	Client(std::string IP, int PORT);
	bool Connect();

	bool SendString(std::string& _string);
	bool CloseConnection();

	sf::CircleShape host;
	sf::CircleShape playerOneShape;
	sf::CircleShape playerTwoShape;


	sf::Vector2f playerPos[3];

	int currentChaser;
	int timeAliveDisplay = 0;

	sf::Font font;
	sf::Text timeAlive;

private: //Private functions
	bool ProcessPacket(Packet _packettype);
	static void ClientThread();
	//Sending Funcs
	bool sendall(char* data, int totalbytes);
	bool SendInt(int _int);
	bool SendPacketType(Packet _packettype);

	//Getting Funcs
	bool recvall(char* data, int totalbytes);
	bool GetInt(int& _int);
	bool GetPacketType(Packet& _packettype);
	bool GetString(std::string& _string);

private:
	SOCKET Connection;//This client's connection to the server
	SOCKADDR_IN addr; //Address to be binded to our Connection socket
	int sizeofaddr = sizeof(addr); //Need sizeofaddr for the connect function
};

static Client* clientptr;