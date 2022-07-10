#pragma once
#include <memory>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <OGL3D/Math/OVec3.h>
#include <OGL3D/Math/OVec2.h>
#include <OGL3D/Math/OVec4.h>
//#include <OGL3D/Math/OQuaternion.h>

//#include <OGL3D/Math/OMat4.h>

class OVertexArrayObject;
class OUniformBuffer;
class OShaderProgram;
class OTexture;
class Shader;
class Camera;
class Mesh;
class ModelLoader;
class ModelAnimation;
class Animators;
class Animation;


typedef std::shared_ptr<OVertexArrayObject> OVertexArrayObjectPtr;
typedef std::shared_ptr<OUniformBuffer> OUniformBufferPtr;
typedef std::shared_ptr<OShaderProgram> OShaderProgramPtr;
typedef std::shared_ptr<OTexture> OTexturePtr;
typedef std::shared_ptr<Shader> ShaderPtr;
typedef std::shared_ptr<Camera> OCameraPtr;
typedef std::shared_ptr<Mesh> OMeshPtr;
typedef std::shared_ptr<ModelLoader> OModelPtr;

typedef std::shared_ptr<ModelAnimation> OModelAnimationPtr;
typedef std::shared_ptr<Animation> OAnimationPtr;
typedef std::shared_ptr<Animators> OAnimatorPtr;




typedef float f32;
typedef int i32;
typedef unsigned int ui32;



#define MAX_BONE_INFLUENCE 4


struct Vertex {
	// position
	OVec3 Position;
	// normal
	//OVec4 Color;

	OVec3 Normal;
	// texCoords
	OVec2 TexCoords;
	// tangent
	OVec3 Tangent;
	// bitangent
	OVec3 Bitangent;
	//bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
	unsigned int id=0;
	std::string type="";
	std::string path="";
};


enum CameraMovementDesc {
	FORWARD=0,
	BACKWARD,
	LEFT,
	RIGHT,
	
};

struct TextureSizeDesc
{
	i32 height=0;
	i32 width = 0;
};
struct OVertexAttribute
{
	ui32 numElements = 0;

};

struct OVertexBufferDesc
{
	void* verticesList = nullptr;
	void* indicesList = nullptr;
	
	ui32 stride = 0;
	
	int size_of_vertices=0;
	ui32 size_of_indices=0;
	OVertexAttribute* attributesList = nullptr;
	ui32 attributesListSize = 0;

	bool tesselation = false;
	ui32 num_patches = 0;
};

struct OShaderProgramDesc
{
	const wchar_t* vertexShaderFilePath;
	const wchar_t* fragmentShaderFilePath;

};
struct ShaderDesc
{
	const char* vsFilePath;
	const char* fsFilePath;
	const char* gsFilePath=nullptr;
	const char* tcsFilePath = nullptr;
	const char* tesFilePath = nullptr;

};


struct OTextureDesc
{
	const char* textureFilePath=nullptr;
	ui32 num_of_texture=0;
	const char* tex_type=nullptr;
	bool cubemap = false;

	std::vector<std::string> faces = {
	"Assets/Images/skybox/diuso/right.jpg",
	"Assets/Images/skybox/diuso/left.jpg",
	"Assets/Images/skybox/diuso/top.jpg",
	"Assets/Images/skybox/diuso/bottom.jpg",
	"Assets/Images/skybox/diuso/front.jpg",
	"Assets/Images/skybox/diuso/back.jpg"
	};
};

struct OUniformBufferDesc
{
	ui32 size = 0;
};

enum OTriangleType
{
	TriangleList = 0,
	TriangleStrip
};


enum OShaderType
{
	VertexShader = 0,
	FragmentShader
};


#define OGL3D_ERROR(message)\
{\
std::stringstream m;\
m << "OGL3D Error: " << message << std::endl;\
throw std::runtime_error(m.str());\
}

#define OGL3D_WARNING(message)\
std::wclog << "OGL3D Warning: " << message << std::endl;


#define OGL3D_INFO(message)\
std::wclog << "OGL3D Info: " << message << std::endl;