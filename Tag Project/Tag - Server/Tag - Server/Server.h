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

#include <SFML/Graphics.hpp>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <string>
#include <iostream>

enum Packet
{
	P_playerInput
};

class Server
{
public:
	Server(int PORT, bool BroadcastPublically = false);
	bool ListenForNewConnection();

	sf::CircleShape host;
	sf::CircleShape playerOneShape;
	sf::CircleShape playerTwoShape;

	sf::Vector2f playerPos[3];

	int currentChaser = 0;
	int speed = 10;

	int hitDelay = 30;
	int hitDelayCap = 90;
	int playerHit = -1;

	int timer = 0;
	int hostAliveTime = 0;
	int playerOneAliveTime = 0;
	int playerTwoAliveTime = 0;

	std::string positionsString; // All player positions

	sf::Font font;
	sf::Text timeAlice;
	int totalConnections = 0;

	bool SendString(int ID, std::string& _string);

	void sendNewPos(); // Update positions on client side
	void sendChaser(); // Tell clients current chaser

private:
	bool sendall(int ID, char* data, int totalbytes);
	bool recvall(int ID, char* data, int totalbytes);
	bool SendInt(int ID, int _int);
	bool GetInt(int ID, int& _int);
	bool SendPacketType(int ID, Packet _packettype);
	bool GetPacketType(int ID, Packet& _packettype);
	bool GetString(int ID, std::string& _string);
	bool ProcessPacket(int ID, Packet _packettype);
	static void ClientHandlerThread(int ID);

	void updatePlayerPos(int ID, std::string& Input);

	void collisions();

	void displayAliveTimer();

	int elims = 0;

private:
	SOCKET Connections[100];
	int TotalConnections = 0;

	SOCKADDR_IN addr; //Address that we will bind our listening socket to
	int addrlen = sizeof(addr);
	SOCKET sListen;
};

static Server* serverptr; //Serverptr is necessary so the static ClientHandler method can access the server instance/functions.
