#pragma once
#include <OGL3D/OPrerequisities.h>
#include "OVec2.h"
#include "OVec3.h"
#include "OVec4.h"

#include "OMat4.h"

#include "OQuaternion.h"

#include <memory>
#include <vector>

#include<assimp/quaternion.h>
#include<assimp/vector3.h>
#include<assimp/matrix4x4.h>

namespace AssimpMathHelpers
{
	static  OMat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
	{
		OMat4 to;
		//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		to.m_mat[0][0] = from.a1; to.m_mat[1][0] = from.a2; to.m_mat[2][0] = from.a3; to.m_mat[3][0] = from.a4;
		to.m_mat[0][1] = from.b1; to.m_mat[1][1] = from.b2; to.m_mat[2][1] = from.b3; to.m_mat[3][1] = from.b4;
		to.m_mat[0][2] = from.c1; to.m_mat[1][2] = from.c2; to.m_mat[2][2] = from.c3; to.m_mat[3][2] = from.c4;
		to.m_mat[0][3] = from.d1; to.m_mat[1][3] = from.d2; to.m_mat[2][3] = from.d3; to.m_mat[3][3] = from.d4;
		return to;
	}

	static  OVec3 GetGLMVec(const aiVector3D& vec)
	{
		return OVec3(vec.x, vec.y, vec.z);
	}
	static  OVec4 GetGLMColor(aiColor4D* vec)
	{
		return OVec4(1, 1, 1, 1);//9vec->r, vec->g, vec->b, vec->a);
	}

	static  OQuaternion GetGLMQuat(const aiQuaternion& pOrientation)
	{
		return OQuaternion(pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z);
	}
}	

