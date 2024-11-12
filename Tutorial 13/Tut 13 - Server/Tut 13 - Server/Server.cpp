#include "Server.h"
#include <iostream>
#include "PacketStructs.h"
#pragma comment(lib,"ws2_32.lib") // Required for WinSock

// Handles communication with a single client connection in a separate thread
void Server::ClientHandlerThread(Server& server, std::shared_ptr<Connection> connection)
{
	PacketType packettype;
	while (true)
	{
		if (server.m_terminateThreads) // Check if the server has signaled to terminate threads
			break;
		if (!server.GetPacketType(connection, packettype)) // Retrieve the packet type
			break; // Exit loop if there is an issue retrieving packet type
		if (!server.ProcessPacket(connection, packettype)) // Process the packet based on type
			break; // Exit loop if there is an issue processing the packet
	}
	std::cout << "Lost connection to client ID: " << connection->m_ID << std::endl;
	server.DisconnectClient(connection); // Disconnect and clean up this client connection
	return;
}

// Sends pending packets to all clients in a separate thread
void Server::PacketSenderThread(Server& server)
{
	while (true)
	{
		if (server.m_terminateThreads) // Check if the server has signaled to terminate threads
			break;
		std::shared_lock<std::shared_mutex> lock(server.m_mutex_connectionMgr); // Lock connections manager for safe access

		for (auto conn : server.m_connections) // Iterate through each active connection
		{
			if (conn->m_pm.HasPendingPackets()) // Check if there are packets waiting to be sent for this connection
			{
				std::shared_ptr<Packet> p = conn->m_pm.Retrieve(); // Retrieve the next packet in the queue
				if (!server.sendall(conn, (const char*)(&p->m_buffer[0]), p->m_buffer.size())) // Send packet to client
				{
					std::cout << "Failed to send packet to ID: " << conn->m_ID << std::endl; // Log error if sending fails
				}
			}
		}
		Sleep(5); // Pause briefly to allow other threads to process
	}
	std::cout << "Ending Packet Sender Thread..." << std::endl;
}

// Disconnects a client and cleans up resources related to the connection
void Server::DisconnectClient(std::shared_ptr<Connection> connection)
{
	std::lock_guard<std::shared_mutex> lock(m_mutex_connectionMgr); // Lock connections manager for safe removal
	connection->m_pm.Clear(); // Clear any remaining packets in this client's packet queue
	closesocket(connection->m_socket); // Close the client socket
	m_connections.erase(std::remove(m_connections.begin(), m_connections.end(), connection), m_connections.end()); // Remove connection from active connections vector
	std::cout << "Cleaned up client: " << connection->m_ID << "." << std::endl;
	std::cout << "Total connections: " << m_connections.size() << std::endl;
}
