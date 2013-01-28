#include "EntityDeletionPacket.h"

EntityDeletionPacket::EntityDeletionPacket()
{
	networkIdentity = -1;
}

EntityDeletionPacket::~EntityDeletionPacket()
{

}

Packet EntityDeletionPacket::pack()
{
	Packet packet;
	packet << networkIdentity;
	return packet;
}

void EntityDeletionPacket::unpack( Packet p_packet )
{
	p_packet >> networkIdentity;
}
