// =======================================================================================
//                                      TcpServer
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief description...
///        
/// # TcpServer
/// Detailed description...
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <queue>
#include <vector>

#include "ThreadSafeMessaging.h"
#include "Packet.h"

namespace boost
{
	namespace asio
	{
		class io_service;
	};
};

using namespace std;

class TcpCommunicationProcess;
class TcpListenerProcess;

class TcpServer: public ThreadSafeMessaging
{
public:
	TcpServer();
	TcpServer(const string& p_serverName);
	~TcpServer();

	void shutdown();
	void startListening( int p_port );

	void stopListening();

	bool isListening();

	bool hasNewConnections();
	unsigned int newConnectionsCount();
	int popNewConnection();

	unsigned int activeConnectionsCount();
	vector< int > getActiveConnections();

	bool hasNewDisconnections();
	unsigned int newDisconnectionsCount();
	int popNewDisconnection();

	bool hasNewPackets();
	unsigned int newPacketsCount();
	Packet popNewPacket(); // Deprecated and obsolete.

	Packet& getFrontPacket();
	void popFrontPacket();

	void processMessages();

	/// Broadcasts a packet to all connected clients
	void broadcastPacket( Packet& p_packet, int p_excludeClientId=-1 );
	void multicastPacket( vector<int> p_connectionIdentities, Packet& p_packet );
	/// Unicast sends a packet to only one specified client
	void unicastPacket( Packet& p_packet, int p_clientId );
	void unicastPacketQueue( queue<Packet> p_packets, int p_clientId );
	const unsigned int& getTotalBroadcasts();
	void askForCommProcessInfo();
	const unsigned int& totalSentInCommProcess(const unsigned int& p_processIdentity);

	string getServerName();

private:
	void giveBroadcastPacketAUniqueIdentifier( Packet* p_packet );

private:
	bool m_isListening;

	queue<int> m_newConnectionProcesses;
	queue<int> m_newDisconnectionProcesses;
	vector<TcpCommunicationProcess*> m_communicationProcesses;
	// Store total number of sent packets in comm processes.
	vector<unsigned int> m_totalSentInCommProcesses;

	queue<Packet> m_newPackets;

	TcpListenerProcess* m_listenerProcess;
	boost::asio::io_service* m_ioService;

	// Will be increased by 1 for every sent packet.
	unsigned int m_uniqueBroadcastPacketIdentifier;

	string m_serverName;
	
};