#version 430

layout(location = 0) in vec3 particlePosition;
layout(location = 1) in vec3 particleVelocity;
layout(location = 2) in float particleAge;
layout(location = 3) in vec3 particleColor;

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