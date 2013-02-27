#pragma once
#include <Component.h>
// =======================================================================================
// AnomalyBomb
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # AnomalyBomb
/// Detailed description...
/// Created on: 27-2-2013 
///---------------------------------------------------------------------------------------
class AnomalyBomb: public Component
{
public:
	AnomalyBomb(float p_lifeTime, float p_explodeTime, float p_radius,
		float p_eventHorizonRadius);

public:
	float lifeTime;
	float explodeTime;
	float radius;
	float eventHorizonRadius;

};