#version 410 core

layout(location = 0) in vec3 pos;
//layout(location = 1) in vec4 col;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 tex;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;
layout(location = 5) in ivec4 boneIds; 
layout(location = 6) in vec4 weights;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
    
    mat4 viewModel =view * model;
    gl_Position =  projection * viewModel * vec4(pos,1.0f);
    
    FragPos = vec3(model * vec4(pos, 1.0));
    Normal = mat3(transpose(inverse(model))) * norm;  
	TexCoords = tex;
}