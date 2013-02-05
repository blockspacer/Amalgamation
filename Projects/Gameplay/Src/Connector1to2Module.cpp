#include "Connector1to2Module.h"

Connector1to2Module::Connector1to2Module()
	: Component( ComponentType::Connector1to2Module )
{
}

Connector1to2Module::Connector1to2Module(ConnectionPoint p_target1, ConnectionPoint p_target2):
	Component( ComponentType::Connector1to2Module ),
	m_target1(p_target1), m_target2(p_target2)
{
}
Connector1to2Module::~Connector1to2Module()
{

}