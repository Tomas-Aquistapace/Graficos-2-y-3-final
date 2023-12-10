#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoor;
layout (location = 2) in vec3 normal;

out vec2 texCoord;
out vec3 fposition;
out vec3 fnormal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;	

void main()
{
gl_Position = Projection * View * Model * vec4(position, 1.0);
fnormal = mat3(transpose(inverse(Model))) * normal;
fposition = vec4(Model*vec4(position,1.0f)).xyz;
texCoord = texCoor;
}