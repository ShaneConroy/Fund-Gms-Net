#include "Server.h"

Server::Server(int PORT, bool BroadcastPublically) //Port = port to broadcast on. BroadcastPublically = false if server is not open to the public (people outside of your router), true = server is open to everyone (assumes that the port is properly forwarded on router settings)
{
	if (!font.loadFromFile("BebasNeue.otf"))
	{
		std::cout << "Error loading font" << "\n";
	}
	timeAlice.setFont(font);
	timeAlice.setCharacterSize(25);

	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) //If WSAStartup returns anything other than 0, then that means an error has occured in the WinSock Startup.
	{
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	if (BroadcastPublically) //If server is open to public
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else //If server is only for our router
		addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Broadcast locally
	addr.sin_port = htons(PORT); //Port
	addr.sin_family = AF_INET; //IPv4 Socket

	sListen = socket(AF_INET, SOCK_STREAM, NULL); //Create socket to listen for new connections
	if (bind(sListen, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) //Bind the address to the socket, if we fail to bind the address..
	{
		std::string ErrorMsg = "Failed to bind the address to our listening socket. Winsock Error:" + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	if (listen(sListen, SOMAXCONN) == SOCKET_ERROR) //Places sListen socket in a state in which it is listening for an incoming connection. Note:SOMAXCONN = Socket Oustanding Max Connections, if we fail to listen on listening socket...
	{
		std::string ErrorMsg = "Failed to listen on listening socket. Winsock Error:" + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	serverptr = this;
}

bool Server::ListenForNewConnection()
{
	SOCKET newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen); //Accept a new connection
	if (newConnection == 0) //If accepting the client connection failed
	{
		std::cout << "Failed to accept the client's connection." << std::endl;
		return false;
	}
	else //If client connection properly accepted
	{
		std::cout << "Client Connected! ID:" << TotalConnections << std::endl;
		Connections[TotalConnections] = newConnection; //Set socket in array to be the newest connection before creating the thread to handle this client's socket.
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(TotalConnections), NULL, NULL); //Create Thread to handle this client. The index in the socket array for this thread is the value (i).
		std::string MOTD = "MOTD: Welcome! This is the message of the day!.";
		SendString(TotalConnections, MOTD);
		TotalConnections += 1; //Incremenent total # of clients that have connected
		return true;
	}
}

bool Server::ProcessPacket(int ID, Packet _packettype)
{
	switch (_packettype)
	{
	case P_Message: // Players input
	{
		std::string Message;
		if (!GetString(ID, Message))
		{
			return false;
		}
		std::cout << "Proccesing packet" << "\n";
		updatePlayerPos(ID, Message);

		std::cout << Message << " from, ID: " << ID << "\n";

		break;
	}

	default: //If packet type is not accounted for
	{
		std::cout << "Unrecognized packet: " << _packettype << std::endl; //Display that packet was not found
		break;
	}
	}
	return true;
}

void Server::ClientHandlerThread(int ID) //ID = the index in the SOCKET Connections array
{
	Packet PacketType;
	while (true)
	{
		if (!serverptr->GetPacketType(ID, PacketType)) //Get packet type
			break; //If there is an issue getting the packet type, exit this loop
		if (!serverptr->ProcessPacket(ID, PacketType)) //Process packet (packet type)
			break; //If there is an issue processing the packet, exit this loop
	}
	std::cout << "Lost connection to client ID: " << ID << std::endl;
	closesocket(serverptr->Connections[ID]);
	return;
}

void Server::updatePlayerPos(int ID, std::string& Input)
{
	// If there is two seperate keyboards
	
	//if ((ID == 0) || (ID == 1))
	//{
	//	if (Input == "w" || Input == "W")
	//	{
	//		playerPos[ID + 1].y -= speed;
	//	}
	//	if (Input == "s" || Input == "S")
	//	{
	//		playerPos[ID + 1].y += speed;
	//	}
	//	if (Input == "a" || Input == "A")
	//	{
	//		playerPos[ID + 1].x -= speed;
	//	}
	//	if (Input == "d" || Input == "D")
	//	{
	//		playerPos[ID + 1].x += speed;
	//	}
	//}

	// If there is only one keyboard

	if (ID == 0)
	{
		if (Input == "w" || Input == "W")
		{
			playerPos[1].y -= speed;
		}
		if (Input == "s" || Input == "S")
		{
			playerPos[1].y += speed;
		}
		if (Input == "a" || Input == "A")
		{
			playerPos[1].x -= speed;
		}
		if (Input == "d" || Input == "D")
		{
			playerPos[1].x += speed;
		}
	}

	if (ID == 1)
	{
		if (Input == "i" || Input == "I")
		{
			playerPos[2].y -= speed;
		}
		if (Input == "k" || Input == "K")
		{
			playerPos[2].y += speed;
		}
		if (Input == "j" || Input == "J")
		{
			playerPos[2].x -= speed;
		}
		if (Input == "l" || Input == "L")
		{
			playerPos[2].x += speed;
		}
	}

	// Screen wrapping
	for (int iter = 0; iter <= 2; iter++)
	{
		if (playerPos[iter].x > 800)
		{
			playerPos[iter].x = -100;
		}
		else if (playerPos[iter].x < -100)
		{
			playerPos[iter].x = 800;
		}
		else if (playerPos[iter].y > 600)
		{
			playerPos[iter].y = -100;
		}
		else if (playerPos[iter].y < -100)
		{
			playerPos[iter].y = 600;
		}
	}
	host.setPosition(playerPos[0]);

	// After updating the positions, send them out
	collisions();
	sendNewPos();
}

void Server::collisions()
{
	if (hitDelay <= 0)
	{
		if (host.getGlobalBounds().intersects(playerOneShape.getGlobalBounds()))
		{
			if (currentChaser == 0)
			{
				currentChaser = 1;
				hitDelay = hitDelayCap;
			}
			else if (currentChaser == 1)
			{
				currentChaser = 0;
				hitDelay = hitDelayCap;
			}
		}
		if (host.getGlobalBounds().intersects(playerTwoShape.getGlobalBounds()))
		{
			if (currentChaser == 0)
			{
				currentChaser = 2;
				hitDelay = hitDelayCap;
			}
			else if (currentChaser == 2)
			{
				currentChaser = 0;
				hitDelay = hitDelayCap;
			}
		}
		if (playerOneShape.getGlobalBounds().intersects(playerTwoShape.getGlobalBounds()))
		{
			if (currentChaser == 1)
			{
				currentChaser = 2;
				hitDelay = hitDelayCap;
			}
			else if (currentChaser == 2)
			{
				currentChaser = 1;
				hitDelay = hitDelayCap;
			}
		}
		sendChaser();
	}

	if (currentChaser == 0)
	{
		host.setFillColor(sf::Color::Red);
		playerOneShape.setFillColor(sf::Color::Green);
		playerTwoShape.setFillColor(sf::Color::Green);
	}
	if (currentChaser == 1)
	{
		host.setFillColor(sf::Color::Green);
		playerOneShape.setFillColor(sf::Color::Red);
		playerTwoShape.setFillColor(sf::Color::Green);
	}
	if (currentChaser == 2)
	{
		host.setFillColor(sf::Color::Green);
		playerOneShape.setFillColor(sf::Color::Green);
		playerTwoShape.setFillColor(sf::Color::Red);
	}
}

void Server::sendChaser()
{
	std::string whosChasingOutput = "CHASING/";
	whosChasingOutput += std::to_string(currentChaser);

	for (int i = 0; i < TotalConnections; i++)
	{
		SendString(i, whosChasingOutput);
	}
}

void Server::sendNewPos()
{
    // Update all players' new positions
    host.setPosition(playerPos[0]);
    playerOneShape.setPosition(playerPos[1]);
    playerTwoShape.setPosition(playerPos[2]);

    // Log positions
    logPositions();

    // Send new positions to each client
    std::string positionsString = formatPositionsToString();
    for (int i = 0; i < TotalConnections; i++)
    {
        SendString(i, positionsString);
    }
}

void Server::logPositions()
{
    std::cout << "----Host X: " << host.getPosition().x << ", Host Y: " << host.getPosition().y << "\n";
    std::cout << "----One X: " << playerOneShape.getPosition().x << ", One Y: " << playerOneShape.getPosition().y << "\n";
    std::cout << "----Two X: " << playerTwoShape.getPosition().x << ", Two Y: " << playerTwoShape.getPosition().y << "\n";
}

std::string Server::formatPositionsToString()
{
    std::ostringstream newPositions;
    newPositions << "POS/"
                 << static_cast<int>(playerPos[0].x) << "/"
                 << static_cast<int>(playerPos[0].y) << "/"
                 << static_cast<int>(playerPos[1].x) << "/"
                 << static_cast<int>(playerPos[1].y) << "/"
                 << static_cast<int>(playerPos[2].x) << "/"
                 << static_cast<int>(playerPos[2].y) << "/";

    return newPositions.str();
}



