#pragma once
#include <OGL3D/OPrerequisities.h>
#include <OGL3D/Math/OMat4.h>


class OShaderProgram
{
public:
	OShaderProgram(const OShaderProgramDesc& desc);
	~OShaderProgram();
	ui32 getId();
	void setUniformBufferSlot(const char* name, ui32 slot);
	void setInt(const std::string& name, int value) const;
	void setMat4(const std::string& name, const ::OMat4& mat) const;
private:
	void attach(const wchar_t* shaderFilePath, const OShaderType& type);
	void link();
private:
	ui32 m_programId = 0;
	ui32 m_attachedShaders[2] = {};
	
};

