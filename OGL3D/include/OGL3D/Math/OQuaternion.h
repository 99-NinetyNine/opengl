#pragma once
//#include <OGL3D/OPrerequisities.h>

#include "OVec4.h"
#include "OVec3.h"
#include "OMat4.h"
#include <memory>
#include <vector>

#define DELTA 0.00001

class OQuaternion
{
public:
	OQuaternion()
	{
		m_x = 0;
		m_y = 0;
		m_z = 0;
		m_w = 0;
	}
	OQuaternion(float w,float x, float y, float z) :m_w(w),m_x(x), m_y(y), m_z(z)
	{
	}
	OQuaternion(const OQuaternion& vector) :m_x(vector.m_x), m_y(vector.m_y), m_z(vector.m_z),m_w(vector.m_w)
	{
	}
public:
	//overloading
	void operator =(const OQuaternion& q)
	{

		m_x = (q.m_x);
		m_y = (q.m_y);
		m_z = (q.m_z);
		m_w = (q.m_w);

	}
	OQuaternion operator +(const OQuaternion& q)const
	{
		OQuaternion res;
		res.m_x += m_x+(q.m_x);
		res.m_y += m_y + (q.m_y);
		res.m_z += m_z + (q.m_z);
		res.m_w += m_w + (q.m_w);
		return res;	

	}
	OQuaternion operator *(float k) const
	{
		OQuaternion res;
		res.m_x *= k;
		res.m_y *= k;
		res.m_z *= k;
		res.m_w *= k;
		return res;

	}
	OQuaternion operator /(float num) const
	{
		return OQuaternion(m_w*num,m_x * num, m_y * num, m_z * num);
	}
	OQuaternion operator -()const
	{
	//#unary operator operloainf
		OQuaternion res;
		res.m_x = -m_x;
		res.m_y = -m_y;
		res.m_z = -m_z;
		res.m_w = -m_w;
		return res;
	}

public:
	float static dot(const OQuaternion& x, const OQuaternion& y)
	{
		float res;
		res =
			x.m_x * y.m_x +
			x.m_y * y.m_y +
			x.m_z * y.m_z +
			x.m_w * y.m_w;

		return res;



	}


	static OQuaternion normalize(OQuaternion& q)
	{
		OQuaternion res;
		float mag = (q.m_x) * (q.m_x) + (q.m_y) * (q.m_y) + (q.m_z) * (q.m_z) + (q.m_w) * (q.m_w);
		mag = sqrt(mag);
		if (!mag)
		{
			return res;
		}
		res.m_x = (q.m_x) /mag;
		res.m_y = (q.m_y) /mag;
		res.m_z = (q.m_z) /mag;
		res.m_w = (q.m_w) /mag;

		return res;
	}
	

  	static OQuaternion slerp(const OQuaternion& from, const OQuaternion& to, float t)
	{
		//QuatSlerp(QUAT * from, QUAT * to, float t, QUAT * res)
		OQuaternion res;
		float to1[4];
		double omega, cosom, sinom, scale0, scale1;
			// calc cosine
		float ax = from.m_x;
		float ay = from.m_y;
		float az = from.m_z;
		float aw = from.m_w;
		float bx = to.m_x;
		float by = to.m_y;
		float bz = to.m_z;
		float bw = to.m_w;

		cosom = ax * bx + ay * by + az * bz + aw * bw;
			// adjust signs (if necessary)

			if ((1.0f-cosom) < 0.0) {

				to1[0] = (ax - bx) * t;
				to1[1] = (ay - by) * t;
				to1[2] = (az - bz) * t;
				to1[3] = (aw - bw) * t;
				
				/*cosom = -cosom; 
				to1[0] = -to.m_x;
				to1[1] = -to.m_y;
				to1[2] = -to.m_z;
				to1[3] = -to.m_w;
				*/
			}
			else {


				to1[0] = (ax + bx) * t;
				to1[1] = (ay + by) * t;
				to1[2] = (az + bz) * t;
				to1[3] = (aw + bw) * t;
				/*to1[0] = to.m_x;
				to1[1] = to.m_y;
				to1[2] = to.m_z;
				to1[3] = to.m_w;*/
			}
			res.m_x = to1[0];
			res.m_y = to1[1];
			res.m_z = to1[2];
			res.m_w = to1[3];
			return res;

			// calculate coefficients
			if ((1.0 - cosom) > DELTA) {
				// standard case (slerp)
				omega = acos(cosom);
				sinom = sin(omega);
				scale0 = sin((1.0 - t) * omega) / sinom;
				scale1 = sin(t * omega) / sinom;
			}
			else {
				// "from" and "to" quaternions are very close 
				//  ... so we can do a linear interpolation
				scale0 = 1.0 - t;
				scale1 = t;
			}
			// calculate final values
			res.m_x = scale0 * from.m_x + scale1 * to1[0];
			res.m_y = scale0 * from.m_y + scale1 * to1[1];
			res.m_z = scale0 * from.m_z + scale1 * to1[2];
			res.m_w = scale0 * from.m_w + scale1 * to1[3];
			return res;
	
		//OQuaternion z; 
		//z = y;

		//float cosTheta = dot(x, y);

		//// If cosTheta < 0, the interpolation will take the long way around the sphere.
		//// To fix this, one quat must be negated.
		//if (cosTheta < 0)
		//{
		//	z = -y;
		//	//z = z * -1;
		//	cosTheta = -cosTheta;
		//}
		//

		//// Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle) becoming a zero denominator
		//if (cosTheta > 1)
		//{
		//	// Linear interpolation
		//	return OQuaternion(
		//		mix(x.m_w, z.m_w, a),
		//		mix(x.m_x, z.m_x, a),
		//		mix(x.m_y, z.m_y, a),
		//		mix(x.m_z, z.m_z, a));
		//}
		//else
		//{
		//	// Essential Mathematics, page 467
		//	float angle = acos(cosTheta);
		//	return ((x * sin((1 - a) * angle) + z * sin(a * angle)) / sin(angle));
		//	
		//}
}

	static OQuaternion lol_slerp(OQuaternion qa, OQuaternion qb, float t) {
		// quaternion to return
		OQuaternion qm;
		// Calculate angle between them.
		float cosHalfTheta =static_cast<float>( (qa.m_w) * (qb.m_w) + (qa.m_x) * (qb.m_x) + (qa.m_y) * (qb.m_y) + (qa.m_z) * (qb.m_z));
		// if qa=qb or qa=-qb then theta = 0 and we can return qa
		if (abs(cosHalfTheta) >= 1.0) {
			qm.m_w = qa.m_w;qm.m_x = qa.m_x;qm.m_y = qa.m_y;qm.m_z = qa.m_z;
			return qm;
		}
		// Calculate temporary values.
		float halfTheta = (float)acos(cosHalfTheta);
		float sinHalfTheta = static_cast<float>(sqrt(1.0 - cosHalfTheta * cosHalfTheta));
		// if theta = 180 degrees then result is not fully defined
		// we could rotate around any axis normal to qa or qb
		if (fabs(sinHalfTheta) < 0.001) { // fabs is floating point absolute
			qm.m_w = (qa.m_w * 0.5 + qb.m_w * 0.5);
			qm.m_x = (qa.m_x * 0.5 + qb.m_x * 0.5);
			qm.m_y = (qa.m_y * 0.5 + qb.m_y * 0.5);
			qm.m_z = (qa.m_z * 0.5 + qb.m_z * 0.5);
			return qm;
		}
		float ratioA = static_cast<float>(sin((1 - t) * halfTheta) / sinHalfTheta);
		float ratioB = static_cast<float>(sin(t * halfTheta) / sinHalfTheta);
		//calculate Quaternion.
		qm.m_w = (qa.m_w * ratioA + qb.m_w * ratioB);
		qm.m_x = (qa.m_x * ratioA + qb.m_x * ratioB);
		qm.m_y = (qa.m_y * ratioA + qb.m_y * ratioB);
		qm.m_z = (qa.m_z * ratioA + qb.m_z * ratioB);
		return qm;

	}

	static float mix(float x, float y, float a)
	{
		return (x * (1 - a) + y * a);
	}

	~OQuaternion()
	{
	}
public:
	float m_x, m_y, m_z, m_w;

};