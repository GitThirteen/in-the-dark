#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

layout (location = 3) uniform mat4 transform;
layout (location = 4) uniform mat4 viewProjMat;

out vec3 fragPos;
out vec3 vNormal;
//out vec2 uvCoord;
out vec4 vertexColor;

void main() {
	// replace mat4 with transform once necessary
	gl_Position = viewProjMat * mat4(1.0) * vec4(position, 1.0);
	vertexColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	//fragPos = vec3(mat4(1.0) * vec4(position, 1.0));
	//vNormal = mat3(transpose(inverse(mat4(1.0)))) * normal;
	//uvCoord = uv;
}