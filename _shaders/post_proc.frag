#version 430

in vec2 uvCoords;

layout(location = 2) uniform int screenWidth;
layout(location = 3) uniform int screenHeight;

layout(location = 4) uniform sampler2D colorTex;
layout(location = 5) uniform sampler2D depthTex;

layout(location = 6) uniform float brightness;

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

// =========== RGB HSV CONVERSION =========== //

// Color space conversions taken from lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
vec3 rgb2hsv(vec3 col) {
	vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
	vec4 p = col.g < col.b ? vec4(col.bg, K.wz) : vec4(col.gb, K.xy);
	vec4 q = col.r < p.x ? vec4(p.xyw, col.r) : vec4(col.r, p.yzx);

	float d = q.x - min(q.w, q.y);
	float e = 1.0e-10;
	return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 col) {
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(col.xxx + K.xyz) * 6.0 - K.www);
    return col.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), col.y);
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

	vec4 result = (texture(colorTex, uvCoords) * vec4(edgeData, 1.0) * vig);

	if (brightness != 1.0) {
		vec3 hsv = rgb2hsv(result.rgb);
		hsv.z *= brightness;
		result = vec4(hsv2rgb(hsv), result.w);
	}

	fragColor = result;
}