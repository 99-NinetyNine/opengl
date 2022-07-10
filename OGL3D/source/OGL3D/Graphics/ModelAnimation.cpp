#include <OGL3D/Graphics/ModelAnimation.h>
#include <OGL3D/Math/AssimpMath.h>
#include <assert.h>

ModelAnimation::ModelAnimation(string const& path, bool gamma=false)
{
	stbi_set_flip_vertically_on_load(true);
	gammaCorrection = gamma;
	loadModel(path);
	OGL3D_INFO("Model animation loaded successfully:" << path.c_str() << "\n");
}

void ModelAnimation::Draw(ShaderPtr shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}



int& ModelAnimation::GetBoneCount()
{
	return m_BoneCounter; 
}


void ModelAnimation::loadModel(string const& path)
{
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		OGL3D_ERROR("ERROR::ASSIMP:: " << importer.GetErrorString() << endl);
		return;
	}
	// retrieve the directory path of the filepath
	directory = path.substr(0, path.find_last_of('/'));

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);
}

void ModelAnimation::processNode(aiNode* node, const aiScene* scene)
{
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

void ModelAnimation::SetVertexBoneDataToDefault(Vertex& vertex)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		vertex.m_BoneIDs[i] = -1;
		vertex.m_Weights[i] = 0.0f;
	}
}

Mesh ModelAnimation::processMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		SetVertexBoneDataToDefault(vertex);
		vertex.Position = AssimpMathHelpers::GetGLMVec(mesh->mVertices[i]);
		vertex.Normal = AssimpMathHelpers::GetGLMVec(mesh->mNormals[i]);

		OVec4 col;

		if (mesh->HasVertexColors(i))
		{
			col.m_x = mesh->mColors[i]->r;
			col.m_y = mesh->mColors[i]->g;
			col.m_z = mesh->mColors[i]->b;
			col.m_w = mesh->mColors[i]->a;
		}
		//vertex.Color = OVec4(0.2,0.2,0.1,1.0f);

		//mesh->m*/

		if (mesh->mTextureCoords[0])
		{
			OVec2 vec;
			vec.m_x = mesh->mTextureCoords[0][i].x;
			vec.m_y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = OVec2(0.1f, 0.5f);

		vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];



	vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	int counter = 1;
	if (textures.size() == 0 &&directory.substr(directory.find_last_of('/') + 1) == "source1")
	{
		std::cout << "rocket\n";
	}
	else
	{
	while (textures.size() == 0 && counter <= 21)
	{
		//try everything
		if (counter == 18)
		{
			counter++;
			continue;

		}
		diffuseMaps = loadMaterialTextures(material, aiTextureType(counter), "texture_ambient");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		counter++;
	}
	if (textures.size() == 0)
	{
		//this is iron man hahaha
		//this model's texture is idk what 
		//so anyhow load  it
		load_iron_man = true;
		diffuseMaps = loadMaterialTextures(material, aiTextureType(1), "texture_ambient");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}
	}
	ExtractBoneWeightForVertices(vertices, mesh, scene);

	return Mesh(vertices, indices, textures);

}

void ModelAnimation::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
	{
		if (vertex.m_BoneIDs[i] < 0)
		{
			vertex.m_Weights[i] = weight;
			vertex.m_BoneIDs[i] = boneID;
			break;
		}
	}
}

void ModelAnimation::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
{
	auto& boneInfoMap = m_BoneInfoMap;
	int& boneCount = m_BoneCounter;

	for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
	{
		int boneID = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			BoneInfo newBoneInfo;
			newBoneInfo.id = boneCount;
			newBoneInfo.offset = AssimpMathHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
			boneInfoMap[boneName] = newBoneInfo;
			boneID = boneCount;
			boneCount++;
		}
		else
		{
			boneID = boneInfoMap[boneName].id;
		}
		if (boneID == -1)
		{
			OGL3D_ERROR("boneId=-1");
		}
		auto weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
		{
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			if (vertexId > vertices.size())
			{
				OGL3D_ERROR("line 166 ModelAnimation anim\n");
			}
			SetVertexBoneData(vertices[vertexId], boneID, weight);
		}
	}
}

unsigned int ModelAnimation::TextureFromFile(const char* path, const string& directory, bool gamma)
{
	string filename = string(path);
	filename = filename.substr(filename.find_last_of('/')+1);
	filename = directory + "/textures/"+filename;

	string wild= filename.substr(filename.find_last_of('.') + 1);
	if(wild.compare("tga")==0)
	{
		filename=filename +".png";
	}

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
	
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		OGL3D_WARNING("Texture|" << path<<"failed to load");
		stbi_image_free(data);
	}

	return textureID;
}

vector<Texture> ModelAnimation::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	
	vector<Texture> textures;
	/*
	aiColor4D diffuse;
	if (AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		std::cout << "hi";

	*/


	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. 
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	if (load_iron_man)
	{   // if texture hasn't been loaded already, load it
		std::string str("/Octane_Albedo.jpg");
		Texture texture;
		texture.id = TextureFromFile(str.c_str(), this->directory);
		texture.type = typeName;
		texture.path = str.c_str();
		textures.push_back(texture);
		textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
	}
	return textures;
}
