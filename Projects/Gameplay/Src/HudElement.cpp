#include "HudElement.h"

ComponentRegister<HudElement> HudElement::s_reg("HudElement");

HudElement::HudElement()
	: Component( ComponentType::HudElement )
{
	m_element = "";
	m_changed = true;
	m_value = "0";
	m_componentTypeId = ComponentType::ComponentTypeIdx::HudElement;
}

HudElement::HudElement( string p_element )
	: Component( ComponentType::HudElement )
{
	m_element = p_element;
	m_changed = true;
	m_value = "0";
	m_componentTypeId = ComponentType::ComponentTypeIdx::HudElement;
}


HudElement::~HudElement()
{
}

void HudElement::init( vector<ComponentData> p_initData )
{
	char* cStr;
	for( unsigned int i=0; i<p_initData.size(); i++ )
	{
		if( p_initData[i].dataName == "m_element" )
		{
			p_initData[i].getDataAsCharArray(&cStr);
			m_element = string(cStr);
			m_changed = true;
		}
		else if( p_initData[i].dataName == "m_value" )
		{
			p_initData[i].getDataAsCharArray(&cStr);
			m_value = string(cStr);
			m_changed = true;
		}
	}
}

void HudElement::setValue( string p_value )
{
	if( p_value != m_value )
	{
		m_value = p_value;
		m_changed = true;
	}
}

bool HudElement::hasChanged()
{
	return m_changed;
}

void HudElement::setRead()
{
	m_changed = false;
}

string HudElement::getElement()
{
	return m_element;
}

string HudElement::getValue()
{
	return m_value;
}