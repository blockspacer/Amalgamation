// =======================================================================================
//                                      NetworkSynced
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	NetworkSynced Component.
///        
/// # NetworkSynced
/// Detailed description.....
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <Component.h>

class NetworkSynced: public Component
{
public:
	NetworkSynced()
	{
		m_networkIdentity = -1;
	}

	NetworkSynced( int p_networkIdentity )
	{
		m_networkIdentity = p_networkIdentity;
	}

	~NetworkSynced()
	{
	}

private:
	int m_networkIdentity;

};