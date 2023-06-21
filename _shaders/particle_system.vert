#version 430

layout(location = 17) in vec3 particlePosition;
layout(location = 18) in vec3 particleVelocity;
layout(location = 19) in float particleAge;
layout(location = 20) in vec3 particleColor;

out vec3 pPosition;
out vec3 pVelocity;
out float pAge;
out vec3 pColor;

void main() {
	pPosition = particlePosition;
	pVelocity = particleVelocity;
	pAge = particleAge;
	pColor = particleColor;
}