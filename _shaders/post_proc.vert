#version 430

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 uv;

out vec2 uvCoord;

void main() {
	gl_Position = vec4(position.x, position.y, 0.0, 1.0);
	uvCoord = uv;
}