#include <OGL3D/Graphics/Animators.h>

Animators::Animators()
{

}
Animators::~Animators()
{
}

Animators::Animators(OAnimationPtr animation)
{

	m_CurrentTime = 0.0;
	m_CurrentAnimation = animation;

	m_FinalBoneMatrices.reserve(100);

	for (int i = 0; i < 100; i++)
		m_FinalBoneMatrices.push_back(OMat4(1.0f));
}

void Animators::UpdateAnimation(float dt)
{

	m_DeltaTime = dt;
	if (m_CurrentAnimation)
	{
		m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
		m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
		CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), OMat4(1.0f));
	}
}

void Animators::PlayAnimation(OAnimationPtr pAnimation)
{
	m_CurrentAnimation = pAnimation;
	m_CurrentTime = 0.0f;
}

void Animators::CalculateBoneTransform(const AssimpNodeData* node, OMat4 parentTransform)
{
	std::string nodeName = node->name;
	OMat4 nodeTransform = node->transformation;

	Bone* Bone = m_CurrentAnimation->FindBone(nodeName);

	if (Bone)
	{
		Bone->Update(m_CurrentTime);
		nodeTransform = Bone->GetLocalTransform();
	}

	OMat4 globalTransformation = parentTransform * nodeTransform;

	auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
	if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		int index = boneInfoMap[nodeName].id;
		OMat4 offset = boneInfoMap[nodeName].offset;
		m_FinalBoneMatrices[index] = globalTransformation * offset;
	}

	for (int i = 0; i < node->childrenCount; i++)
		CalculateBoneTransform(&node->children[i], globalTransformation);

}

std::vector<OMat4> Animators::GetFinalBoneMatrices()
{
	return m_FinalBoneMatrices;
}
