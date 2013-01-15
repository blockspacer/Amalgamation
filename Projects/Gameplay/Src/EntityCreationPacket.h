#pragma once
#include "Packetizer.h"

// =======================================================================================
//                                      EntityCreationPacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles the packing and unpacking of entity creations.
///        
/// # EntityCreationPacket
/// Detailed description.....
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------

class EntityCreationPacket : public Packetizer
{
public:
	EntityCreationPacket();
	~EntityCreationPacket();

	virtual Packet pack();

	virtual void unpack( Packet p_packet );

public:
	char			entityType;
	int				owner;
	int				networkIdentity;
	int				meshInfo;
	AglVector3		translation;
	AglVector3		scale;
	AglQuaternion	rotation;
};