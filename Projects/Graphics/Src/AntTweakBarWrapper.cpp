#include "AntTweakBarWrapper.h"
#include <ToString.h>

AntTweakBarWrapper* AntTweakBarWrapper::sInstance = NULL;

AntTweakBarWrapper::AntTweakBarWrapper( ID3D11Device* p_device)
{
	TwInit(TW_DIRECT3D11,p_device);
	m_onlyMouseMovement=true;
	m_overallBar	= TwNewBar(overall.c_str());
	m_soundBar		= TwNewBar(sound.c_str());
	m_graphicsBar	= TwNewBar(graphics.c_str());
	m_input			= TwNewBar(input.c_str());
	m_network		= TwNewBar(network.c_str());
	m_measurement	= TwNewBar(measurement.c_str());

	initBars();
}

AntTweakBarWrapper::~AntTweakBarWrapper()
{
	TwTerminate();
}

void AntTweakBarWrapper::initBars()
{
	TwDefine(" GLOBAL contained=true ");
	TwDefine(" GLOBAL fontresizable=false ");
	defineParametersForABar(overall.c_str()," color='109 158 235' position= '20 50'");
	defineParametersForABar(sound.c_str()," color='142 124 195' iconified=true");
	defineParametersForABar(graphics.c_str()," color='255 217 102' iconified=true");
	defineParametersForABar(input.c_str()," color='224 102 102' iconified=true");
	defineParametersForABar(network.c_str()," color='147 196 125' iconified=true");
	defineParametersForABar(measurement.c_str()," color='147 196 125' position='1020 40' size='260 600'");
	modifyTheRefreshRate(INPUT,0.1f);
}

void AntTweakBarWrapper::render()
{
	TwDraw();
}

AntTweakBarWrapper* AntTweakBarWrapper::getInstance(ID3D11Device* p_device/* =NULL */)
{
	if (sInstance == NULL)
		if(p_device != NULL)
			sInstance = new AntTweakBarWrapper(p_device);
	return sInstance;
}

void AntTweakBarWrapper::destroy()
{
	if (sInstance)	
		delete sInstance;
	sInstance = NULL;
}

bool AntTweakBarWrapper::handleMessage( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if ( TwEventWin(hWnd, message, wParam, lParam) ) // send event message to AntTweakBar
		return true; // event has been handled by AntTweakBar
	return false;
}


void AntTweakBarWrapper::setMousePos( int p_mouseX, int p_mouseY )
{
	TwMouseMotion(p_mouseX,p_mouseY);
}

void AntTweakBarWrapper::setKeyPressed( int p_keyASCII, int p_modifier )
{
	TwKeyPressed(p_keyASCII,p_modifier);
}


void AntTweakBarWrapper::setMouseBtn( int p_action, int p_buttonID )
{
	TwMouseButton((TwMouseAction)p_action, (TwMouseButtonID)p_buttonID);
}

void AntTweakBarWrapper::addReadOnlyVariable(BarType p_barType, const char* p_name, 
											 TwType p_type, const void *p_var, 
											 const char* p_misc)
{
	switch (p_barType)
	{
	case AntTweakBarWrapper::SOUND:
		TwAddVarRO(m_soundBar,p_name,p_type,p_var,p_misc);
		break;
	case AntTweakBarWrapper::GRAPHICS:
		TwAddVarRO(m_graphicsBar,p_name,p_type,p_var,p_misc);
		break;
	case AntTweakBarWrapper::INPUT:
		TwAddVarRO(m_input,p_name,p_type,p_var,p_misc);
		break;
	case AntTweakBarWrapper::NETWORK:
		TwAddVarRO(m_network,p_name,p_type,p_var,p_misc);
		break;
	case AntTweakBarWrapper::MEASUREMENT:
		TwAddVarRO(m_measurement,p_name,p_type,p_var,p_misc);
		break;
	default:
		TwAddVarRO(m_overallBar,p_name,p_type,p_var,p_misc);
		break;
	}
}

void AntTweakBarWrapper::addWriteVariable(BarType p_barType, const char* p_name, 
										  TwType p_type, void *p_var, 
										  const char* p_misc)
{
	switch (p_barType)
	{
	case AntTweakBarWrapper::SOUND:
		TwAddVarRW(m_soundBar,p_name,p_type,p_var,p_misc);
		break;
	case AntTweakBarWrapper::GRAPHICS:
		TwAddVarRW(m_graphicsBar,p_name,p_type,p_var,p_misc);
		break;
	case AntTweakBarWrapper::INPUT:
		TwAddVarRW(m_input,p_name,p_type,p_var,p_misc);
		break;
	case AntTweakBarWrapper::NETWORK:
		TwAddVarRW(m_network,p_name,p_type,p_var,p_misc);
		break;
	case AntTweakBarWrapper::MEASUREMENT:
		TwAddVarRW(m_measurement,p_name,p_type,p_var,p_misc);
		break;
	default:
		TwAddVarRW(m_overallBar,p_name,p_type,p_var,p_misc);
		break;
	}
}

TwBar* AntTweakBarWrapper::getAntBar(BarType p_barType)
{
	TwBar* searchedBar = NULL;
	
	switch (p_barType)
	{
	case AntTweakBarWrapper::SOUND:
		searchedBar = m_soundBar;
		break;
	case AntTweakBarWrapper::GRAPHICS:
		searchedBar = m_graphicsBar;
		break;
	case AntTweakBarWrapper::INPUT:
		searchedBar = m_input;
		break;
	case AntTweakBarWrapper::NETWORK:
		searchedBar = m_network;
		break;
	case AntTweakBarWrapper::MEASUREMENT:
		searchedBar = m_measurement;
		break;
	default:
		searchedBar = m_overallBar;
		break;
	}

	return searchedBar;
}

void AntTweakBarWrapper::defineParametersForABar( const char* p_nameOfBar, 
												 const char* p_params )
{
	string tmp = " ";
	tmp += p_nameOfBar;
	tmp += p_params;
	TwDefine(tmp.c_str());
}

void AntTweakBarWrapper::modifyTheRefreshRate(BarType p_barType, float p_value)
{
	string tmp = " refresh=";
	tmp += toString(p_value);
	switch (p_barType)
	{
	case AntTweakBarWrapper::SOUND:
		defineParametersForABar(sound.c_str(),tmp.c_str());
		break;
	case AntTweakBarWrapper::GRAPHICS:
		defineParametersForABar(graphics.c_str(),tmp.c_str());
		break;
	case AntTweakBarWrapper::INPUT:
		defineParametersForABar(input.c_str(),tmp.c_str());
		break;
	case AntTweakBarWrapper::NETWORK:
		defineParametersForABar(network.c_str(),tmp.c_str());
		break;
	case AntTweakBarWrapper::MEASUREMENT:
		defineParametersForABar(measurement.c_str(),tmp.c_str());
		break;
	default:
		defineParametersForABar(overall.c_str(),tmp.c_str());
		break;
	}
}

void AntTweakBarWrapper::setWindowSize( int p_width, int p_height )
{
	TwWindowSize(p_width, p_height);
}
