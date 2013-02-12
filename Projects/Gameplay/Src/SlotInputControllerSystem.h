#pragma once
#include <EntitySystem.h>
class Control;
class InputActionsBackendSystem;
class InputBackendSystem;
class TcpClient;
// =======================================================================================
// SlotInputControllerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Handle the input for slot highlighting and activation.
///        
/// # SlotInputControllerSystem
/// Detailed description...
/// Created on: 4-2-2013 
///---------------------------------------------------------------------------------------
class SlotInputControllerSystem: public EntitySystem
{
public:
	SlotInputControllerSystem(InputBackendSystem* p_inputBackend, TcpClient* p_client);
	~SlotInputControllerSystem();
	void sendModuleSlotHighlightDeactivateAll();
	void handleSlotSelection();
private:
	void initialize();
	void process();
	void initKeyboard();
	void initGamepad();
	void initMouse();
	void sendModuleSlotHighlight(int p_slot);
	void sendModuleSlotHighlightDeactivate(int p_slot);

	void sendSlotActivation();
	void sendSlotDeactivation();

	void sendSlotRotationAdd();
	void sendSlotRotationSub();
	void sendSlotRotationNone();

private:
	InputActionsBackendSystem* m_actionBackend;
	InputBackendSystem* m_inputBackend;
	TcpClient* m_client;
};