#include "AnomalyBombEffectSystem.h"
#include "AnomalyBombEffectPiece.h"
#include <RandomUtil.h>
#include "EntityParent.h"

AnomalyBombEffectSystem::AnomalyBombEffectSystem()
	: EntitySystem(SystemType::AnomalyBombEffectSystem, 4,
	ComponentType::AnomalyBombEffectPiece, ComponentType::Transform,
	ComponentType::EntityParent, ComponentType::RenderInfo)
{
}

void AnomalyBombEffectSystem::processEntities( const vector<Entity*>& p_entities )
{
	for(unsigned int i=0; i<p_entities.size(); i++)
	{
		AnomalyBombEffectPiece* piece = static_cast<AnomalyBombEffectPiece*>(
			p_entities[i]->getComponent(ComponentType::AnomalyBombEffectPiece));
		piece->lifeTime -= m_world->getDelta() * 1.0f;
		if(piece->lifeTime <= 0.0f)
		{
			while(piece->lifeTime <= 0.0f)
			{
				piece->lifeTime += piece->maxLifeTime;
			}
			RandomUtil::randomEvenlyDistributedSphere(&piece->offsetVector.x,
				&piece->offsetVector.y, &piece->offsetVector.z);
			piece->offsetVector *= piece->maxRadius;
			AglMatrix localTransform;
			AglMatrix::componentsToMatrix(localTransform, AglVector3(1.0f, 1.0f, 1.0f),
				AglQuaternion::rotateToFrom(AglVector3::forward(), piece->offsetVector),
				piece->offsetVector);
			EntityParent* parent = static_cast<EntityParent*>(p_entities[i]->
				getComponent(ComponentType::EntityParent));
			parent->setLocalTransform(localTransform);
		}

		float lifeTimeFraction = piece->lifeTime / piece->maxLifeTime;
		AglMatrix localTransform;
		AglVector3 scale = AglVector3::one() - AglVector3(lifeTimeFraction, lifeTimeFraction, lifeTimeFraction) * 0.75f;
		//AglVector3 scale = AglVector3(lifeTimeFraction, lifeTimeFraction, lifeTimeFraction);
		AglMatrix::componentsToMatrix(localTransform, scale,
			AglQuaternion::rotateToFrom(AglVector3::up(), -piece->offsetVector),
			piece->offsetVector * lifeTimeFraction * lifeTimeFraction);//sqrt(lifeTimeFraction));
		EntityParent* parent = static_cast<EntityParent*>(p_entities[i]->
			getComponent(ComponentType::EntityParent));
		parent->setLocalTransform(localTransform);
	}
}