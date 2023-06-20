#version 430

in vec3 fragPosition;
in vec3 vertexNormal;
in vec2 uvCoord;
in vec4 vertexColor;

// Directional Light
layout(location = 5) uniform vec3 dirLightDirection;
layout(location = 6) uniform vec3 dirLightColor;

// Point Light
struct PointLight {
	vec3 color;
	vec3 position;
	vec3 attenuation; // .x (constant) .y (linear) .z (quadratic)
};

layout(std430, binding = 0) buffer pointLightBuffer {
	PointLight pointLights[];
};

// Camera
layout(location = 10) uniform vec3 camPosition;

// Mesh Properties
layout(location = 11) uniform vec3 reflection; // .x (ambient) .y (diffuse) .z (specular)
layout(location = 12) uniform float glossiness;

// Texture
layout(location = 13) uniform sampler2D tex;

// Screen Properties
layout(location = 14) uniform int screenWidth;
layout(location = 15) uniform int screenHeight;

out vec4 fragColor;

// ************************************************************* //
// ************************************************************* //
// ************************************************************* //

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

// =========== TOON SHADING =========== //

float calcIntensity(float value) {
	if (value > 0.8) return 1.0;
	if (value > 0.5) return 0.8;
	if (value > 0.2) return 0.5;
	if (value > 0.0) return 0.2;
	return 0.0;
}

vec3[2] calcPointLight(PointLight light, vec3 vNormal, vec3 viewDir) {
	float d = distance(fragPosition, light.position);
	float a = light.attenuation.x + (light.attenuation.y * d) + (light.attenuation.z * (d * d));

	float TORCH_LIGHT_MULTIPLIER = 5;

	vec3 lightDir = normalize(light.position - fragPosition);
	float intensity = smoothstep(0.0, 0.01, max(dot(vNormal, lightDir), 0.0)) * TORCH_LIGHT_MULTIPLIER;
	vec3 diffuse = (intensity * light.color) / a;

	vec3 reflDirection = reflect(-lightDir, vNormal);
	float reflDot = max(dot(viewDir, reflDirection) * intensity, 0.0);
	float reflIntensity = smoothstep(0.0, 0.01, pow(reflDot, glossiness * glossiness));
	vec3 specular = (reflIntensity * light.color) / a;

	vec3 result[2];
	result[0] = diffuse;
	result[1] = specular;
	return result;
}

// =========== MAIN =========== //

// Values taken from https://en.wikipedia.org/wiki/Canny_edge_detector
int[25] gaussianFilter = int[] (
	2,  4,  5,  4, 2,
	4,  9, 12,  9, 4,
	5, 12, 15, 12, 5,
	4,  9, 12,  9, 4,
	2,  4,  5,  4, 2
);
float gaussianFactor = 1.0 / 159.0;

vec3 blurWithGaussian() {
	float offX = 1.0f / float(screenWidth);
	float offY = 1.0f / float(screenHeight);

	vec2[25] offset5x5 = vec2[] (
		vec2(-2*offX,  2*offY), vec2(-offX,  2*offY), vec2(0.0f,  2*offY), vec2(offX,  2*offY), vec2(2*offX,  2*offY),
		vec2(-2*offX,    offY), vec2(-offX,    offY), vec2(0.0f,    offY), vec2(offX,    offY), vec2(2*offX,    offY),
		vec2(-2*offX,    0.0f), vec2(-offX,    0.0f), vec2(0.0f,    0.0f), vec2(offX,    0.0f), vec2(2*offX,    0.0f),
		vec2(-2*offX,   -offY), vec2(-offX,   -offY), vec2(0.0f,   -offY), vec2(offX,   -offY), vec2(2*offX,   -offY),
		vec2(-2*offX, -2*offY), vec2(-offX, -2*offY), vec2(0.0f, -2*offY), vec2(offX, -2*offY), vec2(2*offX, -2*offY)
	);

	int i;
	int size = gaussianFilter.length();
	vec3 value = vec3(0.0f);
	for (i = 0; i < size; i++) {
		value += vec3(texture(tex, uvCoord + offset5x5[i])) * (gaussianFactor * gaussianFilter[i]);
	}

	return value;
}

void main() {
	vec3 vertNormal = normalize(vertexNormal);
	vec3 viewDirection = normalize(camPosition - fragPosition);

	// Ambient
	vec3 ambient = reflection.x * vec3(1.0f);

	// Diffuse (Directional)
	vec3 dLightDir = normalize(-dirLightDirection);
	float dIntensity = smoothstep(0.0, 0.01, max(dot(vertNormal, dLightDir), 0.0));
	vec3 dDiffuse = dIntensity * dirLightColor;

	vec3 diffuse = reflection.y * dDiffuse;

	// Specular (Directional)
	vec3 dReflDirection = reflect(-dLightDir, vertNormal);
	float dReflDot = max(dot(viewDirection, dReflDirection) * dIntensity, 0.0);
	float dReflIntensity = smoothstep(0.0, 0.01, pow(dReflDot, glossiness * glossiness));
	vec3 dSpecular = (dReflIntensity * dirLightColor);

	vec3 specular = reflection.z * dSpecular;

	// Toon Shading
	vec3 hsvL = rgb2hsv(ambient + diffuse);
	float value = calcIntensity(hsvL.z);
	vec3 rgbL = hsv2rgb(vec3(hsvL.xy, value));

	/*hsvL = rgb2hsv(texture(tex, uvCoord).rgb);
	value = calcIntensity(hsvL.z);
	vec3 rgbTex = hsv2rgb(vec3(hsvL.xy, value));*/

	vec3 result = rgbL * texture(tex, uvCoord).rgb + specular;

	// Point Lights
	for (int i = 0; i < pointLights.length(); i++) {
		PointLight light = pointLights[i];
		vec3[2] pointValues = calcPointLight(light, vertNormal, viewDirection);

		// Toon Shading
		vec3 hsvL = rgb2hsv(pointValues[0]);
		float value = calcIntensity(hsvL.z);
		vec3 rgbL = hsv2rgb(vec3(hsvL.rg, value));

		/*hsvL = rgb2hsv(texture(tex, uvCoord).rgb);
		value = calcIntensity(hsvL.z);
		vec3 rgbTex = hsv2rgb(vec3(hsvL.xy, value));*/

		result += rgbL * texture(tex, uvCoord).rgb + pointValues[1];
	}

	fragColor = vec4(result, 1.0);
}