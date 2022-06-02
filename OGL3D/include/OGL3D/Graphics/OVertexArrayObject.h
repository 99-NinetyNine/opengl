#pragma once
#include <OGL3D/OPrerequisities.h>
#include <iostream>
#include <vector>
class OVertexArrayObject
{
public:
	OVertexArrayObject();
	OVertexArrayObject(const OVertexBufferDesc& data);
	~OVertexArrayObject();

	ui32 getId();
	ui32 getLightId();

	ui32 getVertexBufferSize();
	ui32 getVertexSize();
public:
	//handles VAO

	void genarateVAO();
	void LinkAttribute(ui32 layout, ui32 numComponents,ui32 stride, void* offset);
	void BindVAO();
	void UnbindVAO();
	void DeleteVAO();
public:
	//mesh-- related to vbo

	void generateVBO(std::vector<Vertex>& vertices);
	void BindVBO();
	void UnbindVBO();
	void DeleteVBO();
public:
	//related to ebo
	void generateEBO(std::vector<ui32>& indices);
	void BindEBO();
	void UnbindEBO();
	void DeleteEBO();
private:
	ui32 m_vertexBufferId = 0;
	ui32 m_vertexArrayObjectId = 0;
	ui32 m_elementArrayObjectId = 0;
	ui32 m_lightArrayObjectId = 0;
	OVertexBufferDesc m_vertexBufferData;
	bool m_isElementArray = false;;
};

