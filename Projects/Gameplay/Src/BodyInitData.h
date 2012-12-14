#pragma once
#include <Component.h>
#include <AglVector3.h>
#include <AglQuaternion.h>

class BodyInitData : public Component
{
public:
	BodyInitData(AglVector3 p_position, AglQuaternion p_orientation,
					AglVector3 p_scale, AglVector3 p_velocity,
					AglVector3 p_angularVelocity, int p_type, bool p_static = false)
	{
		m_position = p_position;
		m_orientation = p_orientation;
		m_scale = p_scale;
		m_velocity = p_velocity;
		m_angularVelocity = p_angularVelocity;
		m_type = p_type;
		m_static = p_static;
	}
	AglVector3 m_position;
	AglQuaternion m_orientation;
	AglVector3 m_scale;
	AglVector3 m_velocity;
	AglVector3 m_angularVelocity;

	int m_type;

	bool m_static;
};