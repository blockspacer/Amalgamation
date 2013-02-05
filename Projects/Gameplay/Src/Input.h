// =======================================================================================
//                                      Input
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # Input
/// Detailed description.....
/// Created on: 5-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <Component.h>

class Input : public Component
{
public:
	Input()
		: Component( ComponentType::Input )
	{
		m_bBtnPressed = false;
	}
	virtual ~Input(){};

	bool m_bBtnPressed;
};