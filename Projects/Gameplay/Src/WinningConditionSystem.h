#pragma once
#include <EntitySystem.h>
class TcpServer;
class ServerStateSystem;
class PlayersWinLosePacket;
// =======================================================================================
//	WinningConditionSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # WinningConditionSystem
/// Detailed description...
/// Created on: 30-1-2013 
///---------------------------------------------------------------------------------------

class WinningConditionSystem: public EntitySystem
{
public:
	WinningConditionSystem(TcpServer* p_server);
	~WinningConditionSystem();
	void process();

	void setEndTime(float p_endTime);

	int getRemaningMinutes() const;
	int getRemaningSeconds() const;

private:
	vector< pair<float, int> > createSortedScoreEntityMapping();
	///-----------------------------------------------------------------------------------
	/// Sends a packet stating if a client won or lost. Currently only handles one winner.
	/// \param pair<float
	/// \param p_scoreComponentMapping
	/// \return void
	///-----------------------------------------------------------------------------------
	void signalEndSession(vector< pair<float, int> > p_scoreComponentMapping);

	void calculateWinners(PlayersWinLosePacket* p_packet);

	virtual void initialize();


private:
	float m_endTime;
	float m_elapsedGameSessionTime;
	TcpServer* m_server;
	ServerStateSystem* m_stateSystem;
};