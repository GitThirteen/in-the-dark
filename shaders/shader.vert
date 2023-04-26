#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

layout(location = 3) uniform mat4 transform;
layout(location = 4) uniform mat4 viewProjMat;

//out vec3 fragPos;
//out vec3 vNormal;
//out vec2 uvCoord;

void main() {
	gl_Position = viewProjMat * transform * vec4(position, 1.0);
	
	//fragPos = vec3(transform * vec4(position, 1.0));
	//vNormal = mat3(transpose(inverse(transform))) * normal;
	//uvCoord = uv;
}