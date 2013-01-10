#pragma once

#include <EntitySystem.h>

class Control;
class TcpClient;
class PhysicsSystem;
class ShipInputProcessingSystem;

// =======================================================================================
//                                ShipControllerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	System for controlling a ship
///        
/// # ShipControllerSystem
/// Detailed description.....
/// Created on: 13-12-2012 
///---------------------------------------------------------------------------------------

class ShipFlyControllerSystem : public EntitySystem
{
public:
	ShipFlyControllerSystem(ShipInputProcessingSystem* p_inputBackend,
						 PhysicsSystem* p_physicsSystem,
						 TcpClient* p_client );
	~ShipFlyControllerSystem();

	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );

private:
	//Anton - 9/1-13
	float getSpeedBoost(Entity* p_entity, float p_baseThrust);

private:
	PhysicsSystem* m_physics;
	TcpClient* m_client;

	ShipInputProcessingSystem* m_shipInput;	
};