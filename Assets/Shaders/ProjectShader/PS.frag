#version 410 core
out vec4 FragColors;

struct SpotLight {
    vec3 position;
    vec3 direction;
  };
uniform SpotLight spotLight;
void main()
{
    FragColors = vec4(1.0f);
}

