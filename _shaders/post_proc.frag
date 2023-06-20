#version 430

in vec2 uvCoord;

layout(location = 14) uniform int screenWidth;
layout(location = 15) uniform int screenHeight;

uniform sampler2D tex;

out vec4 fragColor;

int[9] laplacian = int[] (
	-1, -1, -1,
	-1,  8, -1,
	-1, -1, -1
);

void main() {
	// Vignette
	vec2 dims = vec2(screenWidth, screenHeight);
	vec2 center_pos = (gl_FragCoord.xy / dims.xy) - vec2(0.5);
	center_pos.x *= dims.x / dims.y;
	float len = length(center_pos);
	float vig = smoothstep(0.95, 0.3, len);

	// Contour via Laplacian
	// Personal Note:
	// Maybe switch to Canny + Otsu's in the future, but for now, this is (hopefully) good enough.
	float offX = 1.0f / float(screenWidth);
	float offY = 1.0f / float(screenHeight);

	vec2[9] offset3x3 = vec2[] (
		vec2(-offX,  offY), vec2(0.0f,  offY), vec2(offX,  offY),
		vec2(-offX,  0.0f), vec2(0.0f,  0.0f), vec2(offX,  0.0f),
		vec2(-offX, -offY), vec2(0.0f, -offY), vec2(offX, -offY)
	);

	int i;
	int size = laplacian.length();
	vec3 edgeDetection = vec3(0.0f);
	for (i = 0; i < size; i++) {
		edgeDetection += vec3(texture(tex, uvCoord + offset3x3[i])) * laplacian[i];
	}

	vec4 t = texture(tex, uvCoord);
	fragColor = vec4(edgeDetection, 1.0f);
}