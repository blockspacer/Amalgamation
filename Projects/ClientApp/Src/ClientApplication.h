#pragma once

/************************************************************************/
/* This defines if a local server should be created or not. Simply add	*/
/* a _ before to use the local server. Otherwise start a separate server*/
/* by compiling the server app and run it.								*/
/************************************************************************/
//#define _COMBINE_CLIENT_AND_SERVER

/************************************************************************/
/* Enables or disables the sound creation for easy use of instead of	*/
/* removing code														*/
/************************************************************************/
#define _ENABLE_SOUND

/************************************************************************/
/* ????	-Robin & Alex													*/
/************************************************************************/
#define WIN32_LEAN_AND_MEAN // Johan: dafuq?

/************************************************************************/
/* There is a problem where boost must initialize some socket-			*/
/* related stuff before other things. Therefore TcpClient should		*/
/* be included as soon as possible. Johan: Haven't looked too			*/
/* much into this yet.													*/
/************************************************************************/
#include <TcpClient.h> 
#include <Globals.h>
#include <windows.h>

class EntityWorld;
class TcpClient;

#ifdef _COMBINE_CLIENT_AND_SERVER
namespace Srv
{
	class ServerApplication;
};
#endif // _COMBINE_CLIENT_AND_SERVER

// =======================================================================================
//	ClientApplication
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief The client application run all systems related to the client logic.
///        
/// # ClientApplication
/// Detailed description...
/// Created on: 11-12-2012 
///---------------------------------------------------------------------------------------


class ClientApplication
{
public:
	ClientApplication( HINSTANCE p_hInstance );
	~ClientApplication();

	void run();

private:
	void initSystems();
	void initEntities();
	void initSounds();
	void initSoundSystem();
private:
	HINSTANCE m_hInstance;

	bool m_running;

	EntityWorld* m_world;
	TcpClient* m_client;
#ifdef _COMBINE_CLIENT_AND_SERVER
	Srv::ServerApplication* m_serverApp;
#endif
};