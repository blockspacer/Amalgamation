#pragma once
#include <EntitySystem.h>
class Control;
// =======================================================================================
// InputActionsBackendSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief A system to use to get certain input states as an action instead of a key press
/// or other input methods.
///        
/// # InputActionsBackendSystem
/// Detailed description...
/// Created on: 11-2-2013 
///---------------------------------------------------------------------------------------
class InputActionsBackendSystem: public EntitySystem
{
public:
	enum Actions
	{
		Actions_NON_EXISTING = -1,
		
		Actions_ACTIVATE_SLOT_1,
		Actions_ACTIVATE_SLOT_2,
		Actions_ACTIVATE_SLOT_3,
		Actions_ACTIVATE_SLOT_4,
		Actions_ROTATE_MODULE_LEFT,
		Actions_ROTATE_MODULE_RIGHT,
		Actions_STRAFE_LEFT,
		Actions_STRAFE_RIGHT,
		Actions_STRAFE_UP,
		Actions_STRAFE_DOWN,
		Actions_TURN_LEFT,
		Actions_TURN_RIGHT,
		Actions_TURN_UP,
		Actions_TURN_DOWN,
		Actions_CURSOR_LEFT,
		Actions_CURSOR_RIGHT,
		Actions_CURSOR_UP,
		Actions_CURSOR_DOWN,
		Actions_ROLL_LEFT,
		Actions_ROLL_RIGHT,
		Actions_THRUST_FORWARD,
		Actions_TRIGGER_EDIT_MODE,
		Actions_MOVE_MODULE,
		Actions_DROP_MODULE,

		Actions_CNT//End Actions enum
	};

	InputActionsBackendSystem(string p_inputIniFile);
	~InputActionsBackendSystem();
	void initialize();
	double getDeltaByAction(Actions p_action);

private:
	Control* m_mappedKeyboardAndMouseInputControls[Actions_CNT];
	Control* m_mappedGamepadInputControls[Actions_CNT];
	string m_inputIniFile;

};