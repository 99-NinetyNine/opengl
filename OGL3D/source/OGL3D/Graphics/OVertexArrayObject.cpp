#include <OGL3D/Graphics/OVertexArrayObject.h>
#include <glad/glad.h>

OVertexArrayObject::OVertexArrayObject()
{
}


OVertexArrayObject::OVertexArrayObject(const OVertexBufferDesc& data)
{
	//if (!data.listSize) OGL3D_ERROR("OVertexArrayObject | listSize is NULL");
	if (!data.stride) OGL3D_ERROR("OVertexArrayObject | vertexSize is NULL");
	if (!data.verticesList) OGL3D_ERROR("OVertexArrayObject | verticesList is NULL");
	//if (!data.indicesList) OGL3D_ERROR("OVertexArrayObject | indicedList is NULL");
	
	glGenVertexArrays(1, &m_vertexArrayObjectId);
	glBindVertexArray(m_vertexArrayObjectId);

	glGenBuffers(1, &m_vertexBufferId);	
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);

	//if element array object
	if (data.indicesList != nullptr) 
	{
		glGenBuffers(1, &m_elementArrayObjectId);
		m_isElementArray = true;
	}
	else {
		m_isElementArray = false;
	}

	glBufferData(GL_ARRAY_BUFFER, data.size_of_vertices, data.verticesList, GL_STATIC_DRAW);

	

	//if element array object
	if (data.indicesList != nullptr)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementArrayObjectId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size_of_indices, data.indicesList, GL_STATIC_DRAW);
	}

	for (ui32 i = 0; i < data.attributesListSize; i++)
	{
		glVertexAttribPointer(
			i,
			data.attributesList[i].numElements,
			GL_FLOAT,
			GL_FALSE,
			data.stride,
			(void*)(3*i*sizeof(f32))
		);
		glEnableVertexAttribArray(i);
	}
	//light VAO
	
	/*glGenVertexArrays(1, &m_lightArrayObjectId);
	glBindVertexArray(m_lightArrayObjectId);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);*/

	if (data.tesselation)
	{
		glPatchParameteri(GL_PATCH_VERTICES, data.num_patches);
	}

	m_vertexBufferData = data;
}

OVertexArrayObject::~OVertexArrayObject()
{
	glDeleteVertexArrays(1, &m_vertexArrayObjectId);
	glDeleteBuffers(1, &m_vertexBufferId);
	if (m_isElementArray)
	{
		glDeleteBuffers(1, &m_elementArrayObjectId);
	}
	
}

ui32 OVertexArrayObject::getId()
{
	return m_vertexArrayObjectId;
}

ui32 OVertexArrayObject::getLightId()
{
	return m_lightArrayObjectId;
}

ui32 OVertexArrayObject::getVertexBufferSize()
{
	return m_vertexBufferData.stride;
}

ui32 OVertexArrayObject::getVertexSize()
{
	return m_vertexBufferData.stride;
}


void OVertexArrayObject::genarateVAO()
{
	glGenVertexArrays(1, &m_vertexArrayObjectId);
}

void OVertexArrayObject::LinkAttribute(ui32 layout, ui32 numComponents, ui32 stride, void* offset)
{
	BindVBO();
	glVertexAttribPointer(layout, numComponents, GL_FLOAT, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	UnbindVBO();
}

void OVertexArrayObject::BindVAO()
{
	glBindVertexArray(m_vertexArrayObjectId);
}

void OVertexArrayObject::UnbindVAO()
{
	glBindVertexArray(0);
}

void OVertexArrayObject::DeleteVAO()
{
	glDeleteVertexArrays(1, &m_vertexArrayObjectId);
}

//generates vbo
void OVertexArrayObject::generateVBO(std::vector<Vertex>& vertices)
{
	glGenBuffers(1, &m_vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

// Binds the VBO
void OVertexArrayObject::BindVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
}

// Unbinds the VBO
void OVertexArrayObject::UnbindVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Deletes the VBO
void OVertexArrayObject::DeleteVBO()
{
	glDeleteBuffers(1, &m_vertexBufferId);
}


// generates a Elements Buffer Object and links it to indices
void OVertexArrayObject::generateEBO(std::vector<ui32>& indices)
{
	m_isElementArray = true;
	glGenBuffers(1, &m_elementArrayObjectId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementArrayObjectId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

// Binds the EBO
void OVertexArrayObject::BindEBO()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementArrayObjectId);
}

// Unbinds the EBO
void OVertexArrayObject::UnbindEBO()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Deletes the EBO
void OVertexArrayObject::DeleteEBO()
{
	glDeleteBuffers(1, &m_elementArrayObjectId);
}