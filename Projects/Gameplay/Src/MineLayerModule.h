#pragma once

#include <Component.h>

// =======================================================================================
//	MineLayerModule
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Describes a module that deploys mines
///        
/// # PlayerScore
/// Detailed description...
/// Created on: 4-1-2013 
///---------------------------------------------------------------------------------------

class MineLayerModule: public Component
{
public:
	float m_cooldown;
	MineLayerModule();
	~MineLayerModule();
private:

};