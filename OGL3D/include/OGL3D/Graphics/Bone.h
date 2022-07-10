#pragma once

/* Container for bone data */

#include <vector>
#include <assimp/scene.h>
#include <list>
#include <assert.h>


#include <OGL3D/OPrerequisities.h>
#include <OGL3D/Math/OVec3.h>
#include <OGL3D/Math/OVec2.h>
#include <OGL3D/Math/OMat4.h>
#include <OGL3D/Math/OQuaternion.h>
#include <OGL3D/Math/AssimpMath.h>

struct KeyPosition
{
	OVec3 position;
	float timeStamp;
};

struct KeyRotation
{
	OQuaternion orientation;
	float timeStamp;
};

struct KeyScale
{
	OVec3 scale;
	float timeStamp;
};

class Bone
{
public:
	Bone(const std::string& name, int ID, const aiNodeAnim* channel)
	{
		m_Name=name;
		m_ID=ID;
		m_LocalTransform.setIdentity();
	
		m_NumPositions = channel->mNumPositionKeys;

		for (int positionIndex = 0; positionIndex < m_NumPositions; ++positionIndex)
		{
			aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
			float timeStamp = channel->mPositionKeys[positionIndex].mTime;
			KeyPosition data;
			data.position = AssimpMathHelpers::GetGLMVec(aiPosition);
			data.timeStamp = timeStamp;
			m_Positions.push_back(data);
		}

		m_NumRotations = channel->mNumRotationKeys;
		for (int rotationIndex = 0; rotationIndex < m_NumRotations; ++rotationIndex)
		{
			aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
			float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
			KeyRotation data;
			data.orientation = AssimpMathHelpers::GetGLMQuat(aiOrientation);
			data.timeStamp = timeStamp;
			m_Rotations.push_back(data);
		}

		m_NumScalings = channel->mNumScalingKeys;
		for (int keyIndex = 0; keyIndex < m_NumScalings; ++keyIndex)
		{
			aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
			float timeStamp = channel->mScalingKeys[keyIndex].mTime;
			KeyScale data;
			data.scale = AssimpMathHelpers::GetGLMVec(scale);
			data.timeStamp = timeStamp;
			m_Scales.push_back(data);
		}
	}

	void Update(float animationTime)
	{
		OMat4 translation = InterpolatePosition(animationTime);
		OMat4 rotation = InterpolateRotation(animationTime);
		OMat4 scale = InterpolateScaling(animationTime);
		m_LocalTransform = translation * rotation;
		m_LocalTransform *= scale;
	}
	
	
	OMat4 GetLocalTransform() { return m_LocalTransform; }
	std::string GetBoneName() const { return m_Name; }
	int GetBoneID() { return m_ID; }



	int GetPositionIndex(float animationTime)
	{
		for (int index = 0; index < m_NumPositions - 1; ++index)
		{
			if (animationTime < m_Positions[index + 1].timeStamp)
				return index;
		}
		assert(0);
	}

	int GetRotationIndex(float animationTime)
	{
		for (int index = 0; index < m_NumRotations - 1; ++index)
		{
			if (animationTime < m_Rotations[index + 1].timeStamp)
				return index;
		}
		assert(0);
	}

	int GetScaleIndex(float animationTime)
	{
		for (int index = 0; index < m_NumScalings - 1; ++index)
		{
			if (animationTime < m_Scales[index + 1].timeStamp)
				return index;
		}
		assert(0);
	}


private:

	float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
	{
		float scaleFactor = 0.0f;
		float midWayLength = animationTime - lastTimeStamp;
		float framesDiff = nextTimeStamp - lastTimeStamp;
		scaleFactor = midWayLength / framesDiff;
		return scaleFactor;
	}

	OMat4 InterpolatePosition(float animationTime)
	{
		if (1 == m_NumPositions)
		{
			OMat4 new_mat;
			new_mat.setTranslation(m_Positions[0].position);
			return new_mat;
		}

		int p0Index = GetPositionIndex(animationTime);
		int p1Index = p0Index + 1;
		float scaleFactor = GetScaleFactor(m_Positions[p0Index].timeStamp,
			m_Positions[p1Index].timeStamp, animationTime);
		OVec3 finalPosition = OVec3::lerp(m_Positions[p0Index].position, m_Positions[p1Index].position
			, scaleFactor);
		finalPosition = OVec3::normalize(finalPosition);
		OMat4 new_mat;
		new_mat.setTranslation(finalPosition);
		return new_mat;
	}

	OMat4 InterpolateRotation(float animationTime)
	{
		if (1 == m_NumRotations)
		{
			OQuaternion rotation = OQuaternion::normalize(m_Rotations[0].orientation);
			return (OMat4::mat4_cast(rotation));
		}

		int p0Index = GetRotationIndex(animationTime);
		int p1Index = p0Index + 1;
		float scaleFactor = GetScaleFactor(m_Rotations[p0Index].timeStamp,
			m_Rotations[p1Index].timeStamp, animationTime);
		OQuaternion finalRotation = OQuaternion::slerp(m_Rotations[p0Index].orientation, m_Rotations[p1Index].orientation,scaleFactor);
		finalRotation = OQuaternion::normalize(finalRotation);
		return (OMat4::mat4_cast(finalRotation));
	}

	OMat4 InterpolateScaling(float animationTime)
	{
		if (1 == m_NumScalings)
		{
			OMat4 x;
			x.setScale(m_Scales[0].scale);
			return x;
		}
		int p0Index = GetScaleIndex(animationTime);
		int p1Index = p0Index + 1;
		float scaleFactor = GetScaleFactor(m_Scales[p0Index].timeStamp,
			m_Scales[p1Index].timeStamp, animationTime);
		OVec3 finalScale = OVec3::lerp(m_Scales[p0Index].scale, m_Scales[p1Index].scale
			, scaleFactor);
		OMat4 new_mat;
		new_mat.setScale(finalScale);
		return new_mat;
	}

public:

	std::vector<KeyPosition> m_Positions;
	std::vector<KeyRotation> m_Rotations;
	std::vector<KeyScale> m_Scales;
	int m_NumPositions;
	int m_NumRotations;
	int m_NumScalings;

	OMat4 m_LocalTransform;
	std::string m_Name;
	int m_ID;
};

