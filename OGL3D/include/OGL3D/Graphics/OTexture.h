#pragma once
#include <OGL3D/OPrerequisities.h>

class OTexture
{
public:
	OTexture(const OTextureDesc& desc);
	void normalTexture(const OTextureDesc& desc);
	void cubemapTexture(const OTextureDesc& desc);
	void bindTexture();

	~OTexture();
	ui32 getId();
	ui32 getCount();
	//void setUniformBufferSlot(const char* name, ui32 slot);

	TextureSizeDesc getTextureSizeDesc();
public:
	const char* type;
	const char* path;
private:
	ui32 m_textureId = 0;
	bool m_cubemap = false;
	ui32 m_count = 0;
	TextureSizeDesc m_sizeTexture;
};

