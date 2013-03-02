#pragma once
#include <EntitySystem.h>
#include <AglVector3.h>
// =======================================================================================
// TeslaEffectSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Animates a lighting strike from one entity to another.
/// 
/// # TeslaEffectSystem
/// Created on: 2-3-2013 
///---------------------------------------------------------------------------------------
class TeslaEffectSystem: public EntitySystem
{
public:
	TeslaEffectSystem();
	void processEntities( const vector<Entity*>& p_entities ) final;
	void animateHit(int p_fromEntity, int p_toEntity);
	void animate( const AglVector3& p_sourcePosition, const AglVector3& p_targetPosition );
};