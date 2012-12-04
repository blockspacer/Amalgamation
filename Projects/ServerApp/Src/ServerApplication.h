// =======================================================================================
//                                      ServerApplication
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	The server application is where all server code runs.
///        
/// # ServerApplication
/// ServerApplication inherits the ThreadSafeMessaging which enables it to communication
/// thread safe with other processes.
/// The server application contains its own EntityWorld. It consists of different
/// ES-systems which in turn are containing game logic, physics, network logic etc...
/// It also calls the EntityWorld's update (or rather "tick") method.
/// Created on: 3-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <EntityWorld.h>


class ServerApplication
{
public:
	ServerApplication();
	virtual ~ServerApplication();

	///-----------------------------------------------------------------------------------
	/// The run() method starts the whole application. It runs all the
	/// Let the program's entry point call run().
	/// \returns void
	///-----------------------------------------------------------------------------------
	void run();

private:

	///-----------------------------------------------------------------------------------
	/// Creates and initializes all EntitySystems.
	/// \returns void
	///-----------------------------------------------------------------------------------
	void initSystems();
	
	/// ... Add more ES-initializations below.

private:
	bool m_running;

	EntityWorld* m_world;

};