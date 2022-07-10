
#pragma once
#include "OVec3.h"
#include <vector>
class OVec4
{
public:
	OVec4() :m_x(0), m_y(0), m_z(0), m_w(0)
	{
	}
	OVec4(float x, float y, float z, float w) :m_x(x), m_y(y), m_z(z), m_w(w)
	{
	}
	OVec4(const OVec4& vec) :m_x(vec.m_x), m_y(vec.m_y), m_z(vec.m_z), m_w(vec.m_w)
	{
	}
	OVec4(const OVec3& vec) :m_x(vec.m_x), m_y(vec.m_y), m_z(vec.m_z), m_w(1.0)
	{
	}
	static OVec4 make_vec4(std::vector<float>& vec)
	{
		OVec4 res;
		res.m_x = vec[0];
		res.m_y=vec[1];
		res.m_z=vec[2];
		res.m_w=vec[3];
		return res;
	}

	void cross(OVec4& v1, OVec4& v2, OVec4& v3)
	{
		this->m_x = v1.m_y * (v2.m_z * v3.m_w - v3.m_z * v2.m_w) - v1.m_z * (v2.m_y * v3.m_w - v3.m_y * v2.m_w) + v1.m_w * (v2.m_y * v3.m_z - v2.m_z * v3.m_y);
		this->m_y = -(v1.m_x * (v2.m_z * v3.m_w - v3.m_z * v2.m_w) - v1.m_z * (v2.m_x * v3.m_w - v3.m_x * v2.m_w) + v1.m_w * (v2.m_x * v3.m_z - v3.m_x * v2.m_z));
		this->m_z = v1.m_x * (v2.m_y * v3.m_w - v3.m_y * v2.m_w) - v1.m_y * (v2.m_x * v3.m_w - v3.m_x * v2.m_w) + v1.m_w * (v2.m_x * v3.m_y - v3.m_x * v2.m_y);
		this->m_w = -(v1.m_x * (v2.m_y * v3.m_z - v3.m_y * v2.m_z) - v1.m_y * (v2.m_x * v3.m_z - v3.m_x * v2.m_z) + v1.m_z * (v2.m_x * v3.m_y - v3.m_x * v2.m_y));
	}
	OVec4 operator = (const OVec4& vec)
	{
		m_x = vec.m_x;
		m_y = vec.m_y;
		m_z = vec.m_z;
		m_w = vec.m_w;
		return *this;
	}
	~OVec4()
	{
	}

public:
	float m_x, m_y, m_z, m_w;
};