#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

layout(location = 3) uniform mat4 transform;
layout(location = 4) uniform mat4 viewProjMat;

out vec3 fragPosition;
out vec3 vertexNormal;
//out vec2 uvCoord;
out vec4 vertexColor;

void main() {
	gl_Position = viewProjMat * transform * vec4(position, 1.0);

	fragPosition = vec3(transform * vec4(position, 1.0));
	vertexNormal = mat3(transpose(inverse(transform))) * normal;
	vertexColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);

	//uvCoord = uv;
}