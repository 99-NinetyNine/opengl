#pragma once

#include <map>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <OGL3D/Graphics/Animation.h>
#include <OGL3D/Graphics/Bone.h>
#include <OGL3D/Graphics/bone_struct.h>


#include <OGL3D/OPrerequisities.h>

class Animators
{
public:
	Animators();
	Animators(OAnimationPtr animation);
	void UpdateAnimation(float dt);
	void PlayAnimation(OAnimationPtr pAnimation);
	void CalculateBoneTransform(const AssimpNodeData* node, OMat4 parentTransform);
	std::vector<OMat4> GetFinalBoneMatrices();
	~Animators();

public:
	std::vector<OMat4> m_FinalBoneMatrices;
	OAnimationPtr m_CurrentAnimation;
	float m_CurrentTime;
	float m_DeltaTime;

};

