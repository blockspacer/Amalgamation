#pragma once

#include <Component.h>
#include <DebugUtil.h>

class ParticleSystemEmitter : public Component
{
public:
	ParticleSystemEmitter()
	: Component( ComponentType::ParticleSystemEmitter_DEPRECATED )
	{
	}

	~ParticleSystemEmitter() {}

	vector<unsigned int> particleSystems;
};