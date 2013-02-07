#include "ShipInputProcessingSystem.h"
#include "InputBackendSystem.h"
#include "Control.h"
#include <ValueClamp.h>
#include "AntTweakBarWrapper.h"
#include "HighlightSlotPacket.h"
#include "SimpleEventPacket.h"
#include "AudioBackendSystem.h"
#include "Cursor.h"
#include <Globals.h>


ShipInputProcessingSystem::ShipInputProcessingSystem(InputBackendSystem* p_inputBackend) :
										EntitySystem( SystemType::ShipInputProcessingSystem )
{
	m_angleInputMultiplier = 1000.0f;

	m_controllerEpsilon = 0.15f;
	m_editSwitchTrigReleased = true;
	m_inputBackend = p_inputBackend;
}

void ShipInputProcessingSystem::initialize()
{
	initGamePad();
	initMouse();
	initKeyboard();

	AntTweakBarWrapper::getInstance()->addWriteVariable( AntTweakBarWrapper::INPUT,
		"ControllerEpsilon", TwType::TW_TYPE_FLOAT, getControllerEpsilonPointer(), 
		"min=0.0 max=1.0 step=0.05" );

	AntTweakBarWrapper::getInstance()->addWriteVariable( AntTweakBarWrapper::INPUT,
		"MouseSensitivity", TwType::TW_TYPE_FLOAT, &m_angleInputMultiplier, "");
}

void ShipInputProcessingSystem::process()
{
	// Fetch the status of the various input methods.
	RawInputForces rawInput = readAllInput();
	// processed input
	m_processedInput = rawInput;

	// Apply correction vectors to the analogue sticks.
	//m_processedInput.horizontalInput += static_cast<float>(m_leftStickCorrection[0]);
	//m_processedInput.verticalInput += static_cast<float>(m_leftStickCorrection[1]);

	// Apply a threshold value to eliminate some of the analogue stick noise.
// 	if( abs(m_processedInput.horizontalInput) < m_controllerEpsilon ) 
// 		m_processedInput.horizontalInput = 0;
// 
// 	if( abs(m_processedInput.verticalInput) < m_controllerEpsilon ) 
// 		m_processedInput.verticalInput = 0;
// 
// 	if( abs(m_processedInput.strafeHorizontalInput) < m_controllerEpsilon ) 
// 		m_processedInput.strafeHorizontalInput = 0;
// 
// 	if( abs(m_processedInput.strafeVerticalInput) < m_controllerEpsilon ) 
// 		m_processedInput.strafeVerticalInput = 0;


	// updateAntTweakBar(m_processedInput);
}

void ShipInputProcessingSystem::setCursorVisibility(bool p_show)
{
	if (p_show)
		m_inputBackend->getCursor()->show();
	else
		m_inputBackend->getCursor()->hide();
}


void ShipInputProcessingSystem::resetCursor()
{
	m_inputBackend->getCursor()->reset();
}


ShipInputProcessingSystem::ResultingInputForces& ShipInputProcessingSystem::getProcessedInput()
{
	return m_processedInput;
}

void ShipInputProcessingSystem::initGamePad()
{
	m_gamepadHorizontalPositive	= m_inputBackend->getControlByEnum( 
		InputHelper::Xbox360Analogs_THUMB_LX_POSITIVE);
	m_gamepadHorizontalNegative	= m_inputBackend->getControlByEnum( 
		InputHelper::Xbox360Analogs_THUMB_LX_NEGATIVE);
	m_gamepadVerticalPositive	= m_inputBackend->getControlByEnum( 
		InputHelper::Xbox360Analogs_THUMB_LY_POSITIVE);
	m_gamepadVerticalNegative	= m_inputBackend->getControlByEnum( 
		InputHelper::Xbox360Analogs_THUMB_LY_NEGATIVE);

	m_gamepadRollRight	= m_inputBackend->getControlByEnum( InputHelper::Xbox360Digitals_SHOULDER_PRESS_R);
	m_gamepadRollLeft	= m_inputBackend->getControlByEnum( InputHelper::Xbox360Digitals_SHOULDER_PRESS_L);

	m_gamepadThrust	= m_inputBackend->getControlByEnum( InputHelper::Xbox360Analogs_TRIGGER_R);

	m_gamepadStrafeHorizontalPositive	= m_inputBackend->getControlByEnum( 
		InputHelper::Xbox360Analogs_THUMB_RX_POSITIVE);
	m_gamepadStrafeHorizontalNegative	= m_inputBackend->getControlByEnum( 
		InputHelper::Xbox360Analogs_THUMB_RX_NEGATIVE);
	m_gamepadStrafeVerticalPositive	= m_inputBackend->getControlByEnum(
		InputHelper::Xbox360Analogs_THUMB_RY_POSITIVE);
	m_gamepadStrafeVerticalNegative	= m_inputBackend->getControlByEnum(
		InputHelper::Xbox360Analogs_THUMB_RY_NEGATIVE);

	m_gamepadEditModeHorizontalPos	= m_inputBackend->getControlByEnum( 
		InputHelper::Xbox360Analogs_THUMB_RX_POSITIVE);
	m_gamepadEditModeHorizontalNeg	= m_inputBackend->getControlByEnum( 
		InputHelper::Xbox360Analogs_THUMB_RX_NEGATIVE);
	m_gamepadEditModeVerticalPos	= m_inputBackend->getControlByEnum(
		InputHelper::Xbox360Analogs_THUMB_RY_POSITIVE);
	m_gamepadEditModeVerticalNeg	= m_inputBackend->getControlByEnum(
		InputHelper::Xbox360Analogs_THUMB_RY_NEGATIVE);

	m_gamepadEditModeTrig = m_inputBackend->getControlByEnum( InputHelper::Xbox360Digitals_BTN_BACK );
}


void ShipInputProcessingSystem::initMouse()
{
	m_mouseHorizontalPositive	= m_inputBackend->getControlByEnum( 
		InputHelper::MouseAxes_X_POSITIVE);
	m_mouseHorizontalNegative	= m_inputBackend->getControlByEnum(
		InputHelper::MouseAxes_X_NEGATIVE);
	m_mouseVerticalPositive		= m_inputBackend->getControlByEnum( 
		InputHelper::MouseAxes_Y_POSITIVE);
	m_mouseVerticalNegative		= m_inputBackend->getControlByEnum( 
		InputHelper::MouseAxes_Y_NEGATIVE);
}

void ShipInputProcessingSystem::initKeyboard()
{
	m_keyboardRollRight = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_E);
	m_keyboardRollLeft =m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_Q);

	m_keyboardThrust = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_SPACE);

	m_keyboardStrafeVerticalPos = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_W);
	m_keyboardStrafeVerticalNeg = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_S);
	m_keyboarStrafeHorizontalPos = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_D);
	m_keyboarStrafeHorizontalNeg = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_A);

	m_keyboardEditModeTrig = m_inputBackend->getControlByEnum( InputHelper::KeyboardKeys_C );

	m_keyboardEditModeVerticalPos = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_W);
	m_keyboardEditModeVerticalNeg = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_S);
	m_keyboardEditModeHorizontalPos = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_D);
	m_keyboardEditModeHorizontalNeg = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_A);
}

float* ShipInputProcessingSystem::getControllerEpsilonPointer()
{
	return &m_controllerEpsilon;
}

ShipInputProcessingSystem::RawInputForces ShipInputProcessingSystem::readAllInput()
{
	RawInputForces input;

	// rotate

	input.hPositive = m_gamepadHorizontalPositive->getStatus();
	input.hPositive += m_mouseHorizontalPositive->getStatus()*m_angleInputMultiplier;
	saturate(input.hPositive); // ?

	input.hNegative = m_gamepadHorizontalNegative->getStatus();
	input.hNegative += m_mouseHorizontalNegative->getStatus()*m_angleInputMultiplier;
	saturate(input.hNegative); // ?

	input.vPositive = m_gamepadVerticalPositive->getStatus();
	input.vPositive += m_mouseVerticalPositive->getStatus()*m_angleInputMultiplier;
	saturate(input.vPositive); // ?

	input.vNegative = m_gamepadVerticalNegative->getStatus();
	input.vNegative += m_mouseVerticalNegative->getStatus()*m_angleInputMultiplier;
	saturate(input.vNegative); // ?

	// strafe

	input.shPositive = m_gamepadStrafeHorizontalPositive->getStatus();
	input.shPositive += m_keyboarStrafeHorizontalPos->getStatus();
	saturate(input.shPositive);

	input.shNegative = m_gamepadStrafeHorizontalNegative->getStatus();
	input.shNegative += m_keyboarStrafeHorizontalNeg->getStatus();
	saturate(input.shNegative);

	input.svPositive = m_gamepadStrafeVerticalPositive->getStatus();
	input.svPositive += m_keyboardStrafeVerticalPos->getStatus();
	saturate(input.svPositive);

	input.svNegative = m_gamepadStrafeVerticalNegative->getStatus();
	input.svNegative += m_keyboardStrafeVerticalNeg->getStatus();
	saturate(input.svNegative);

	// edit mode rotate

	input.ehPositive = m_gamepadEditModeHorizontalPos->getStatus();
	input.ehPositive += m_keyboardEditModeHorizontalPos->getStatus();
	saturate(input.ehPositive);

	input.ehNegative = m_gamepadEditModeHorizontalNeg->getStatus();
	input.ehNegative += m_keyboardEditModeHorizontalNeg->getStatus();
	saturate(input.ehNegative);

	input.evPositive = m_gamepadEditModeVerticalPos->getStatus();
	input.evPositive += m_keyboardEditModeVerticalPos->getStatus();
	saturate(input.evPositive);

	input.evNegative = m_gamepadEditModeVerticalNeg->getStatus();
	input.evNegative += m_keyboardEditModeVerticalNeg->getStatus();
	saturate(input.evNegative);

	// roll

	input.rRight = m_gamepadRollRight->getStatus();
	input.rRight += m_keyboardRollRight->getStatus();
	saturate(input.rRight);

	input.rLeft = m_gamepadRollLeft->getStatus();
	input.rLeft += m_keyboardRollLeft->getStatus();
	saturate(input.rLeft);

	// thrust

	input.thrust =  m_gamepadThrust->getStatus();
	input.thrust += m_keyboardThrust->getStatus();
	saturate(input.thrust);

	// edit mode

	double etv = m_gamepadEditModeTrig->getStatus();
	etv += m_keyboardEditModeTrig->getStatus();
	input.stateSwitchTrig=saturate(etv)>0.5;

	if (m_editSwitchTrigReleased)
	{
		if (input.stateSwitchTrig)
		{
			m_editSwitchTrigReleased = false;
		}
	}
	else
	{
		if (!input.stateSwitchTrig)
		{
			m_editSwitchTrigReleased = true;
		}
		input.stateSwitchTrig=false;
	}
	
	return input;
}
