
#pragma once

class OVec2
{
public:
	OVec2() :m_x(0), m_y(0)
	{
	}
	OVec2(float x, float y) :m_x(x), m_y(y)
	{
	}
	OVec2(const OVec2& vector) :m_x(vector.m_x), m_y(vector.m_y)
	{
	}
	void operator =(const OVec2& vec)
	{
		m_x = vec.m_x;
		m_y = vec.m_y;
		
	}
	OVec2 operator *(float num)
	{
		return OVec2(m_x * num, m_y * num);
	}

	OVec2 operator +(OVec2 vec)
	{
		return OVec2(m_x + vec.m_x, m_y + vec.m_y);
	}

	~OVec2()
	{
	}

public:
	float m_x, m_y;
};