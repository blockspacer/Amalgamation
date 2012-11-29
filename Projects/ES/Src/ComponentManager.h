#pragma once

#include "Component.h"
#include "ComponentType.h"
#include "Entity.h"
#include <bitset>
#include <vector>

using namespace std;

class Component;
class ComponentType;
class Entity;

class ComponentManager
{
public:
	ComponentManager();
	~ComponentManager();

	vector<Component*>& getComponentsFor( Entity* p_entity, vector<Component*>& p_fillBag );
	void deleted( Entity* p_entity );

protected:
	void addComponent( Entity* p_entity, ComponentType p_type, Component* p_component );
	void removeComponent( Entity* p_entity, ComponentType p_type );
	Component* getComponent( Entity* p_entity, ComponentType p_type );
	vector<Component*> getComponentsByType( ComponentType p_type );
	void clean();

private:
	void removeComponentsOfEntity( Entity* p_entity );

private:
	vector< vector<Component*> > m_componentsByType;
	vector<Entity*> m_deleted;
};
