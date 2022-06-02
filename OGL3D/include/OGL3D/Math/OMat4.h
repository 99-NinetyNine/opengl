
#pragma once
#include <OGL3D/OPrerequisities.h>
#include "OVec4.h"
#include "OVec3.h"
#include <memory>
#include <vector>
class OMat4
{
public:
	OMat4()
	{
		setIdentity();
	}
	OMat4(f32 x)
	{
		if (x == 1)
		{
			setIdentity();
		}
	}
	static OMat4 make_mat4(std::vector<f32>&  m)
	{
		OMat4 res;
		ui32 k = 0;
		for (ui32 i = 0;i < 4;i++)
		{
			for (ui32 j = 0;j < 4;j++)
			{
				res.m_mat[i][j] = m[k];
				k++;
			}
		}
		return res;
	}
	static OMat4 mat4_cast(const OVec4& q)

	{
		//converts quaternion to mat4
		OMat4 Result(1);

		f32 qxx(q.m_x * q.m_x);
		f32 qyy(q.m_y * q.m_y);
		f32 qzz(q.m_z * q.m_z);
		f32 qxz(q.m_x * q.m_z);
		f32 qxy(q.m_x * q.m_y);
		f32 qyz(q.m_y * q.m_z);
		f32 qwx(q.m_w * q.m_x);
		f32 qwy(q.m_w * q.m_y);
		f32 qwz(q.m_w * q.m_z);

		Result.m_mat[0][0] = f32(1) - f32(2) * (qyy + qzz);
		Result.m_mat[0][1] = f32(2) * (qxy + qwz);
		Result.m_mat[0][2] = f32(2) * (qxz - qwy);

		Result.m_mat[1][0] = f32(2) * (qxy - qwz);
		Result.m_mat[1][1] = f32(1) - f32(2) * (qxx + qzz);
		Result.m_mat[1][2] = f32(2) * (qyz + qwx);

		Result.m_mat[2][0] = f32(2) * (qxz + qwy);
		Result.m_mat[2][1] = f32(2) * (qyz - qwx);
		Result.m_mat[2][2] = f32(1) - f32(2) * (qxx + qyy);
		
		Result.m_mat[3][3] = 1.0f;

		return Result;
	}

	void setIdentity()
	{
		::memset(m_mat, 0, sizeof(float) * 16);
		m_mat[0][0] = 1;
		m_mat[1][1] = 1;
		m_mat[2][2] = 1;
		m_mat[3][3] = 1;
	}

	void setTranslation(const OVec3& translation)
	{
		m_mat[3][0] = translation.m_x;
		m_mat[3][1] = translation.m_y;
		m_mat[3][2] = translation.m_z;
	}

	void setScale(const OVec3& scale)
	{
		m_mat[0][0] = scale.m_x;
		m_mat[1][1] = scale.m_y;
		m_mat[2][2] = scale.m_z;
	}

	void setRotationX(float x)
	{
		m_mat[1][1] = cos(x);
		m_mat[1][2] = sin(x);
		m_mat[2][1] = -sin(x);
		m_mat[2][2] = cos(x);
	}

	void setRotationY(float y)
	{
		m_mat[0][0] = cos(y);
		m_mat[0][2] = -sin(y);
		m_mat[2][0] = sin(y);
		m_mat[2][2] = cos(y);
	}

	void setRotationZ(float z)
	{
		m_mat[0][0] = cos(z);
		m_mat[0][1] = sin(z);
		m_mat[1][0] = -sin(z);
		m_mat[1][1] = cos(z);
	}

	float getDeterminant()
	{
		OVec4 minor, v1, v2, v3;
		float det;

		v1 = OVec4(this->m_mat[0][0], this->m_mat[1][0], this->m_mat[2][0], this->m_mat[3][0]);
		v2 = OVec4(this->m_mat[0][1], this->m_mat[1][1], this->m_mat[2][1], this->m_mat[3][1]);
		v3 = OVec4(this->m_mat[0][2], this->m_mat[1][2], this->m_mat[2][2], this->m_mat[3][2]);


		minor.cross(v1, v2, v3);
		det = -(this->m_mat[0][3] * minor.m_x + this->m_mat[1][3] * minor.m_y + this->m_mat[2][3] * minor.m_z +
			this->m_mat[3][3] * minor.m_w);
		return det;
	}

	void inverse()
	{
		int a, i, j;
		OMat4 out;
		OVec4 v, vec[3];
		float det = 0.0f;

		det = this->getDeterminant();
		if (!det) return;
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (j != i)
				{
					a = j;
					if (j > i) a = a - 1;
					vec[a].m_x = (this->m_mat[j][0]);
					vec[a].m_y = (this->m_mat[j][1]);
					vec[a].m_z = (this->m_mat[j][2]);
					vec[a].m_w = (this->m_mat[j][3]);
				}
			}
			v.cross(vec[0], vec[1], vec[2]);

			out.m_mat[0][i] = pow(-1.0f, i) * v.m_x / det;
			out.m_mat[1][i] = pow(-1.0f, i) * v.m_y / det;
			out.m_mat[2][i] = pow(-1.0f, i) * v.m_z / det;
			out.m_mat[3][i] = pow(-1.0f, i) * v.m_w / det;
		}

		this->setMatrix(out);
	}


	void operator *=(const OMat4& matrix)
	{
		OMat4 out;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				out.m_mat[i][j] =
					m_mat[i][0] * matrix.m_mat[0][j] + m_mat[i][1] * matrix.m_mat[1][j] +
					m_mat[i][2] * matrix.m_mat[2][j] + m_mat[i][3] * matrix.m_mat[3][j];
			}
		}
		setMatrix(out);
	}

	void setMatrix(const OMat4& matrix)
	{
		::memcpy(m_mat, matrix.m_mat, sizeof(float) * 16);
	}
	void operator =(const OMat4& matrix)
	{
		::memcpy(m_mat, matrix.m_mat, sizeof(float) * 16);
	}

	OVec3 getZDirection()
	{
		return OVec3(m_mat[2][0], m_mat[2][1], m_mat[2][2]);
	}
	OVec3 getXDirection()
	{
		return OVec3(m_mat[0][0], m_mat[0][1], m_mat[0][2]);
	}
	OVec3 getTranslation()
	{
		return OVec3(m_mat[3][0], m_mat[3][1], m_mat[3][2]);
	}

	void setPerspectiveFovLH(float fov, float aspect, float znear, float zfar)
	{
		float h = 1.0f / tan(fov / 2.0f);
		float w = h / aspect;
		m_mat[0][0] = w;
		m_mat[1][1] = h;
		m_mat[2][2] = zfar / (zfar - znear);
		m_mat[2][3] = 1.0f;
		m_mat[3][2] = (-znear * zfar) / (zfar - znear);
		m_mat[3][3] = 0.0f;
		
	}

	void setOrthoLH(float width, float height, float near_plane, float far_plane)
	{
		setIdentity();
		m_mat[0][0] = 2.0f / width;
		m_mat[1][1] = 2.0f / height;
		m_mat[2][2] = 1.0f / (far_plane - near_plane);
		m_mat[3][2] = -(near_plane / (far_plane - near_plane));
	}	
	void LookAtMatrix(const OVec3& cam_pos, const OVec3& target_pos, const OVec3& up)
	{
		
		OVec3 x = target_pos;//messy here confused::: right handed assumed
		x-=cam_pos;
		//x = x * -1;
		OVec3 cam_dir = OVec3::normalize(x);
		OVec3 right_axis = OVec3::normalize(OVec3::cross(up, cam_dir));
		OVec3 up_axis = OVec3::cross(cam_dir, right_axis);

		setIdentity();
		m_mat[0][0] = right_axis.m_x;
		m_mat[0][1] = right_axis.m_y;
		m_mat[0][2] = right_axis.m_z;
		
		m_mat[1][0] = up_axis.m_x;
		m_mat[1][1] = up_axis.m_y;
		m_mat[1][2] = up_axis.m_z;
		
		m_mat[2][0] = cam_dir.m_x;
		m_mat[2][1] = cam_dir.m_y;
		m_mat[2][2] = cam_dir.m_z;
		
		
		m_mat[3][0] = -OVec3::dot(right_axis, cam_pos);
		m_mat[3][1] = -OVec3::dot(up_axis, cam_pos);
		m_mat[3][2] = -OVec3::dot(cam_dir, cam_pos);
		
	}
	~OMat4()
	{
	}
public:
	float m_mat[4][4] = {};
};