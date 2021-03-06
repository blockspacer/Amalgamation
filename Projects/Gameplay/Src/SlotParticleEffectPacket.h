#pragma once

#include "EntityType.h"
#include <AglVector3.h>
#include <AglQuaternion.h>
#include "Packetizer.h"

struct SlotParticleEffectPacket: public Packetizer
{
public:
	SlotParticleEffectPacket();

	virtual Packet pack();

	virtual void unpack( Packet& p_packet );

public:
	int			networkIdentity;
	int			slot;
	int			shipSlot;
	bool		active;
	bool		inEditMode;
	AglVector3	translationOffset;
	AglVector3	forwardDirection;
	int			moduleType;
};