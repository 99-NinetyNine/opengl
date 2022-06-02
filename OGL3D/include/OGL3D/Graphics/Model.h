//#pragma once
//#include <json.h>
//#include <OGL3D/OPrerequisities.h>
//#include <OGL3D/Graphics/Mesh.h>
//#include <OGL3D/Graphics/OTexture.h> 
//#include <OGL3D/Math/OVec3.h> 
//#include <OGL3D/Math/OVec4.h> 
//#include <OGL3D/Math/OVec2.h> 
//#include <OGL3D/Math/OMat4.h>
//
//
//#include <string>
//#include<fstream>
//#include<sstream>
//#include<iostream>
//#include<cerrno>
//using json = nlohmann::json;
//
//
//class Model
//{
//public:
//	// Loads in a model from a file and stores tha information in 'data', 'JSON', and 'file'
//	Model(const char* file);
//	//void Draw(ShaderPtr shader_ptr, OCameraPtr camera_ptr);
//public:
//	std::string get_file_contents(const char* filename);
//public:
//	std::vector<Mesh> meshes;
//	std::vector<OMat4> matricesMeshes;
//private:
//	// Variables for easy access
//	const char* file;
//	std::vector<unsigned char> data;
//	json JSON;
//
//	// All the meshes and transformations
//
//	std::vector<OVec3> translationsMeshes;
//	std::vector<OVec4> rotationsMeshes;
//	std::vector<OVec3> scalesMeshes;
//
//	// Prevents textures from being loaded twice
//	std::vector<std::string> loadedTexName;
//	std::vector<Texture> loadedTex;
//private:
//	// Loads a single mesh by its index
//	void loadMesh(unsigned int indMesh);
//
//	// Traverses a node recursively, so it essentially traverses all connected nodes
//	void traverseNode(unsigned int nextNode, OMat4 matrix = OMat4(1.0f));
//
//	// Gets the binary data from a file
//	std::vector<unsigned char> getData();
//	// Interprets the binary data into floats, indices, and textures
//	std::vector<float> getFloats(json accessor);
//	std::vector<GLuint> getIndices(json accessor);
//	std::vector<Texture> getTextures();
//
//	// Assembles all the floats into vertices
//	std::vector<Vertex> assembleVertices
//	(
//		std::vector<OVec3> positions,
//		std::vector<OVec3> normals,
//		std::vector<OVec2> texUVs
//	);
//
//	// Helps with the assembly from above by grouping floats
//	std::vector<OVec2> groupFloatsVec2(std::vector<float> floatVec);
//	std::vector<OVec3> groupFloatsVec3(std::vector<float> floatVec);
//	std::vector<OVec4> groupFloatsVec4(std::vector<float> floatVec);
//
//};
//
