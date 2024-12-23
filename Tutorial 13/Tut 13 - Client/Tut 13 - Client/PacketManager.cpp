#include "PacketManager.h"

// Clear queue
void PacketManager::Clear()
{
	std::lock_guard<std::mutex> lock(m_mutex_packets); //lock mutex so two threads dont access data at same time
	m_queue_packets = std::queue<std::shared_ptr<Packet>>{};//Clear out packet queue
}

// checks if any packages might be pending
bool PacketManager::HasPendingPackets()
{
	std::lock_guard<std::mutex> lock(m_mutex_packets);
	return (!m_queue_packets.empty()); //returns true if packets are pending
}

// Adds package queue
void PacketManager::Append(std::shared_ptr<Packet> p)
{
	std::lock_guard<std::mutex> lock(m_mutex_packets); //lock mutex so two threads dont access data at same time
	m_queue_packets.push(std::move(p)); //Add packet to queue
}

// remove package from the front of the qy=ueeu
std::shared_ptr<Packet> PacketManager::Retrieve()
{
	std::lock_guard<std::mutex> lock(m_mutex_packets); //lock mutex so two threads dont access data at same time
	std::shared_ptr<Packet> p = m_queue_packets.front(); //Get packet from front of queue
	m_queue_packets.pop(); //Remove packet from front of queue
	return p; //Return packet that was removed from the queue
}