#include "Client.h"

bool Client::ProcessPacket(Packet _packettype)
{
	switch (_packettype)
	{
	case P_Message: //If packet is a chat message packet
	{
		// Store Message
		std::string Message;
		if (!GetString(Message)) // Check not empty 
			return false;


		if (Message.rfind("POS/", 0) == 0)
		{
			size_t beginPos = 4; // Start position after intial "/"

			for (int i = 0; i < 3; i++)
			{
				size_t nextSlash;

				nextSlash = Message.find('/', beginPos); // Finds where the next "/" is
				float playerPosX = std::stof(Message.substr(beginPos, nextSlash - beginPos));
				beginPos = nextSlash + 1;

				nextSlash = Message.find('/', beginPos);
				if (nextSlash == std::string::npos)
					break;

				float playerPosY = std::stof(Message.substr(beginPos, nextSlash - beginPos));
				beginPos = nextSlash + 1;

				playerPos[i] = { playerPosX, playerPosY };
			}

			// Sets players new pos
			host.setPosition(playerPos[0]);
			playerOneShape.setPosition(playerPos[1]);
			playerTwoShape.setPosition(playerPos[2]);
		}

		// Updates colours based on current chaser
		if (Message.rfind("CHASING/", 0) == 0)
		{
			currentChaser = Message[8] - '0'; // Returns chaser from string

			// Host
			if (currentChaser == 0)
			{
				host.setFillColor(sf::Color::Red);
				playerOneShape.setFillColor(sf::Color::Green);
				playerTwoShape.setFillColor(sf::Color::White);
			}

			// Player
			if (currentChaser == 1)
			{
				host.setOutlineColor(sf::Color::White);
				playerOneShape.setOutlineColor(sf::Color::Red);
				playerTwoShape.setOutlineColor(sf::Color::White);
			}

			// Player
			if (currentChaser == 2)
			{
				host.setOutlineColor(sf::Color::White);
				playerOneShape.setOutlineColor(sf::Color::White);
				playerTwoShape.setOutlineColor(sf::Color::Red);
			}
		}
		if (Message.rfind("TIME", 0) == 0)
		{
			timeAlive.setString(Message);
			timeAliveDisplay = 90;
		}

		break;
	}
	default:
		std::cout << "Unrecognized packet: " << _packettype << std::endl; //Display that packet was not found
		break;
	}
	return true;
}

void Client::ClientThread()
{
	Packet PacketType;
	while (true)
	{
		if (!clientptr->GetPacketType(PacketType))
			break;
		if (!clientptr->ProcessPacket(PacketType))
			break;
	}
	std::cout << "Lost connection to the server." << std::endl;
	if (clientptr->CloseConnection())
	{
		std::cout << "Socket to the server was closed successfuly." << std::endl;
	}
	else
	{
		std::cout << "Socket was not able to be closed." << std::endl;
	}
}

Client::Client(std::string IP, int PORT)
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(0);
	}

	addr.sin_addr.s_addr = inet_addr(IP.c_str()); //Address (127.0.0.1) = localhost (this pc)
	addr.sin_port = htons(PORT); //Port 
	addr.sin_family = AF_INET; //IPv4 Socket
	clientptr = this; //Update ptr to the client which will be used by our client thread
}

bool Client::Connect()
{
	Connection = socket(AF_INET, SOCK_STREAM, NULL); //Set Connection socket
	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0) //If we are unable to connect...
	{
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	std::cout << "Connected!" << std::endl;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL); //Create the client thread that will receive any data that the server sends.
	return true;
}

bool Client::CloseConnection()
{
	if (closesocket(Connection) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAENOTSOCK) //If socket error is that operation is not performed on a socket (This happens when the socket has already been closed)
			return true; //return true since connection has already been closed

		std::string ErrorMessage = "Failed to close the socket. Winsock Error: " + std::to_string(WSAGetLastError()) + ".";
		MessageBoxA(NULL, ErrorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}