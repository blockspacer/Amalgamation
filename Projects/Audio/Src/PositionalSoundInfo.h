#pragma once
#include <x3daudio.h>
#include <AglVector3.h>

// =======================================================================================
//                                      PositionalSoundInfo
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains the information needed to use a positional sound
///        
/// # PositionalSoundInfo
/// Detailed description.....
/// Created on: 11-12-2012 
///---------------------------------------------------------------------------------------

struct PositionalSoundInfo
{
	X3DAUDIO_EMITTER emitter;
	AglVector3 previousPosition;

	PositionalSoundInfo()
	{

	}
};