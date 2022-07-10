#pragma once
#include <OGL3D/Math/OMat4.h>

struct BoneInfo
{
	/*id is index in finalBoneMatrices*/
	int id;

	/*offset matrix transforms vertex from model space to bone space*/
	OMat4 offset;

};
