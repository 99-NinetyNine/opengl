#pragma once

#include <vector>
#include <map>

#include <assimp/scene.h>

#include <functional>

#include <OGL3D/OPrerequisities.h>
#include <OGL3D/Graphics/bone_struct.h>
#include <OGL3D/Graphics/Bone.h>
#include <OGL3D/Graphics/ModelAnimation.h>

struct AssimpNodeData
{
	OMat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};

class Animation
{
public:
	Animation();

	Animation(const std::string& animationPath, OModelAnimationPtr model);
	

	Bone* FindBone(const std::string& name);

	float GetTicksPerSecond();
	float GetDuration();	
	AssimpNodeData& GetRootNode();
	std::map<std::string, BoneInfo>& GetBoneIDMap();

	~Animation()
	{
	}
private:
	void ReadMissingBones(const aiAnimation* animation, ModelAnimation& model);
	void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);
private:
	float m_Duration=0;
	int m_TicksPerSecond=0;
	std::vector<Bone> m_Bones;
	AssimpNodeData m_RootNode;
	std::map<std::string, BoneInfo> m_BoneInfoMap;
};

