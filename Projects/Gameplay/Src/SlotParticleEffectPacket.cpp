#include "SlotParticleEffectPacket.h"
#include "PacketType.h"

SlotParticleEffectPacket::SlotParticleEffectPacket()
{
	networkIdentity = -1;
	slot = -1;
	active = false;
	translationOffset = AglVector3();
	forwardDirection = AglVector3();
}

Packet SlotParticleEffectPacket::pack()
{
	Packet packet(static_cast<char>(PacketType::SlotParticleEffectPacket));
	packet << networkIdentity
		<< slot
		<< active
		<< translationOffset
		<< forwardDirection;

	return packet;
}

void SlotParticleEffectPacket::unpack( Packet& p_packet )
{
	p_packet	>> networkIdentity
				>> slot
				>> active
				>> translationOffset
				>> forwardDirection;
}