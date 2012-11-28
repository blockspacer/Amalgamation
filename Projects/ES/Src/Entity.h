#pragma once

#include "ComponentType.h"
#include "SystemType.h"

class Entity
{
public:
	Entity(void);
	~Entity(void);

	int getUUID();
	int getIndex();

	bitset<ComponentType::NUM_COMPONENT_TYPES> getComponentBits() { return m_componentBits; }
	void setComponentBits( bitset<ComponentType::NUM_COMPONENT_TYPES> p_componentBits ) { m_componentBits = p_componentBits; }
	
	bitset<SystemType::NUM_SYSTEM_TYPES> getSystemBits() { return m_systemBits; }
	void setSystemBits( bitset<SystemType::NUM_SYSTEM_TYPES> p_systemBits ) { m_systemBits = p_systemBits; }

	bool isEnabled();
	void setEnabled( bool p_enabled );

private:
	// Universal Unique ID. This id is unique in the network and will never be reused. 
	int m_UUID;

	// This id  is used as an index for the components internally. If an entity is deleted
	// from the world its id will be reused for future created entities.
	int m_index;

	bool m_enabled;

	bitset<ComponentType::NUM_COMPONENT_TYPES> m_componentBits;
	bitset<SystemType::NUM_SYSTEM_TYPES> m_systemBits;
};

