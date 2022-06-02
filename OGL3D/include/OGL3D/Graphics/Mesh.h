#pragma once
#include <glad/glad.h>

#include <OGL3D/OPrerequisities.h>
#include <OGL3D/Math/OVec4.h> 
#include <OGL3D/Math/OVec3.h> 
#include <OGL3D/Math/OVec2.h> 
#include <OGL3D/Graphics/OVertexArrayObject.h>
#include <OGL3D/Graphics/Camera.h>
#include <OGL3D/Graphics/Shader.h>
#include <OGL3D/OPrerequisities.h>

#include <string>
#include <vector>
using namespace std;


class Mesh {
public:
    // constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

    // render the mesh
    void Draw(ShaderPtr shader);

private:
    // initializes all the buffer objects/arrays
    void setupMesh();
public:
    // mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    unsigned int VAO;
private:
    // render data 
    unsigned int VBO, EBO;
};

