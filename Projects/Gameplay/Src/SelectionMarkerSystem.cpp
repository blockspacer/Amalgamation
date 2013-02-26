#include "SelectionMarkerSystem.h"
#include "SelectionMarker.h"
#include "RenderInfo.h"
#include "Transform.h"


SelectionMarkerSystem::SelectionMarkerSystem() : 
	EntitySystem( SystemType::SelectionMarkerSystem, 1,
	ComponentType::SelectionMarker)

{
}

SelectionMarkerSystem::~SelectionMarkerSystem()
{

}

void SelectionMarkerSystem::initialize()
{
	m_marker = m_world->createEntity();

	m_marker->addComponent(ComponentType::SelectionMarker, new SelectionMarker());
	m_marker->addComponent(ComponentType::RenderInfo, new RenderInfo(false));
	m_marker->addComponent(ComponentType::Transform, new Transform());

	m_world->addEntity(m_marker);
}

void SelectionMarkerSystem::processEntities( const vector<Entity*>& p_entities )
{
	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		SelectionMarker* marker = static_cast<SelectionMarker*>(p_entities[i]->getComponent(ComponentType::SelectionMarker));

		if (marker->m_targetEntity >= 0)
		{
			Entity* target = m_world->getEntity(marker->m_targetEntity);

			RenderInfo* markerRenderInfo = static_cast<RenderInfo*>(p_entities[i]->getComponent(ComponentType::RenderInfo));
			RenderInfo* targetRenderInfo = static_cast<RenderInfo*>(target->getComponent(ComponentType::RenderInfo));
	
			markerRenderInfo->m_meshId = targetRenderInfo->m_meshId;
			markerRenderInfo->m_shouldBeRendered = true;
		}
	}
}

void SelectionMarkerSystem::setMarkerTarget(int p_target, AglMatrix p_transform)
{
	SelectionMarker* marker = static_cast<SelectionMarker*>(m_marker->getComponent(ComponentType::SelectionMarker));
	Transform* trans = static_cast<Transform*>(m_marker->getComponent(ComponentType::Transform));
	trans->setMatrix(p_transform);
	marker->m_targetEntity = p_target;
}