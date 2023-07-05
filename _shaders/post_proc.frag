#version 430

in vec2 uvCoords;

layout(location = 14) uniform int screenWidth;
layout(location = 15) uniform int screenHeight;

layout(location = 7) uniform sampler2D colorTex;
layout(location = 8) uniform sampler2D depthTex;

out vec4 fragColor;

int[9] laplacian = int[] (
	-1, -1, -1,
	-1,  8, -1,
	-1, -1, -1
);

float linearize(float depth) {
	float zNear = 0.0001f;
	float zFar = 1.0f;

	return zNear * zFar / (zFar + depth * (zNear - zFar));
}

void main() {
	// Vignette
	vec2 dims = vec2(screenWidth, screenHeight);
	vec2 center_pos = (gl_FragCoord.xy / dims.xy) - vec2(0.5);
	center_pos.x *= dims.x / dims.y;
	float len = length(center_pos);
	float vig = smoothstep(0.95, 0.3, len);

	// Contour via Laplacian
	float offX = 1.0f / float(screenWidth);
	float offY = 1.0f / float(screenHeight);

	vec2[9] offset3x3 = vec2[] (
		vec2(-offX,  offY), vec2(0.0f,  offY), vec2(offX,  offY),
		vec2(-offX,  0.0f), vec2(0.0f,  0.0f), vec2(offX,  0.0f),
		vec2(-offX, -offY), vec2(0.0f, -offY), vec2(offX, -offY)
	);

	int size = laplacian.length();
	vec3 edgeData = vec3(0.0f);
	for (int i = 0; i < size; i++) {
		edgeData += vec3(linearize(texture(depthTex, uvCoords + offset3x3[i]).r)) * laplacian[i];
	}
	edgeData = vec3(1.0f) - abs(edgeData); // Invert colors and use abs for better lines

	fragColor = texture(colorTex, uvCoords) * vec4(edgeData, 1.0) * vig;
}