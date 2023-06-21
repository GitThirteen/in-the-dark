#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

layout(location = 3) uniform mat4 transform;
layout(location = 4) uniform mat4 viewProjMat;

layout(location = 17) in vec3 particlePosition;
layout(location = 18) in vec3 particleVelocity;
layout(location = 19) in float particleAge;
layout(location = 20) in vec3 particleColor;

out vec3 fragPosition;
out vec3 vertexNormal;
out vec2 uvCoord;
out vec4 vertexColor;

// Particle System
out float pType;
out vec3 pPosition;
out vec3 pVelocity;
out float pAge;
out vec3 pColor;

void main() {
	gl_Position = viewProjMat * transform * vec4(position, 1.0);

	fragPosition = vec3(transform * vec4(position, 1.0));
	vertexNormal = mat3(transpose(inverse(transform))) * normal;
	uvCoord = uv;
	vertexColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);

	pType = particleType;
	pPosition = particlePosition;
	pVelocity = particleVelocity;
	pAge = particleAge;
	pColor = particleColor;
}