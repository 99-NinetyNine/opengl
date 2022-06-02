#pragma once
//#include <OGL3D/OPrerequisities.h>
typedef float f32;
#include<vector>
class OVec3
{
public:
	OVec3() :m_x(0), m_y(0), m_z(0)
	{
	}
	OVec3(f32 x, f32 y, f32 z) :m_x(x), m_y(y), m_z(z)
	{
	}
	OVec3(const OVec3& vector) :m_x(vector.m_x), m_y(vector.m_y), m_z(vector.m_z)
	{
	}
	
	static OVec3 lerp(const OVec3& start, const OVec3& end, f32 delta)
	{
		OVec3 v;
		v.m_x = start.m_x * (1.0f - delta) + end.m_x * (delta);
		v.m_y = start.m_y * (1.0f - delta) + end.m_y * (delta);
		v.m_z = start.m_z * (1.0f - delta) + end.m_z * (delta);
		return v;
	}

	OVec3 operator *(f32 num)
	{
		return OVec3(m_x * num, m_y * num, m_z * num);
	}
	
	OVec3 operator +(const OVec3& vec)
	{
		return	 OVec3(m_x + vec.m_x, m_y + vec.m_y, m_z + vec.m_z);
	}
	OVec3 operator - (const OVec3& vec)
	{
		return OVec3(m_x - vec.m_x, m_y - vec.m_y, m_z - vec.m_z);
	}
	OVec3 operator +=(const OVec3& vec)
	{
		m_x += vec.m_x;
		m_y += vec.m_y;
		m_z += vec.m_z;
		return *this;

	}
	OVec3 operator -=(const OVec3& vec)
	{
		m_x -= vec.m_x;
		m_y -= vec.m_y;
		m_z -= vec.m_z;
		return *this;
	}
	void operator =(const OVec3& vec)
	{
		m_x = vec.m_x;
		m_y = vec.m_y;
		m_z = vec.m_z;
	}
	f32 length() const
	{
		return (f32)sqrt((m_x * m_x) + (m_y * m_y) + (m_z * m_z));
	}

	static OVec3 normalize(const  OVec3& vec)
	{
		OVec3 res;
		f32 len = (f32)sqrt((f32)(vec.m_x * vec.m_x) + (f32)(vec.m_y * vec.m_y) + (f32)(vec.m_z * vec.m_z));
		if (!len)
			return OVec3();

		res.m_x = vec.m_x / len;
		res.m_y = vec.m_y / len;
		res.m_z = vec.m_z / len;

		return res;
	}
	
	static OVec3 cross(const  OVec3& v1, const  OVec3& v2)
	{
		OVec3 res;
		res.m_x = (v1.m_y * v2.m_z) - (v1.m_z * v2.m_y);
		res.m_y = (v1.m_z * v2.m_x) - (v1.m_x * v2.m_z);
		res.m_z = (v1.m_x * v2.m_y) - (v1.m_y * v2.m_x);
		return res;
	}
	static f32 dot(const OVec3& v1, const  OVec3& v2)
	{
		return v1.m_x * v2.m_x + v1.m_y * v2.m_y + v1.m_z * v2.m_z;
	}

	static OVec3 make_vec3(std::vector<f32>& vertex)
	{
		OVec3 res;
		res.m_x = vertex[0];
		res.m_y = vertex[1];
		res.m_z = vertex[2];
		return res;
	}



	~OVec3()
	{
	}

public:
	f32 m_x, m_y, m_z;
};	