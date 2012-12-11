// =======================================================================================
//                                      InputSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # InputSystem
/// Detailed description.....
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include "CameraInfo.h"
#include "Input.h"
#include <EntitySystem.h>
#include <InputControlFactory.h>
#include <InputManager.h>
#include <MessageLoopFetcher.h>
#include <XInputFetcher.h>

class InputHelper;
class inputControlFactory;
class InputManager;

class InputSystem : public EntitySystem
{
public:
	InputSystem(void);
	~InputSystem(void);

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );

private:
	InputManager* m_inputManager;
	vector<int> m_controlIdxs;
};
