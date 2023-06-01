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

//layout(location = 7) uniform vec3 pntLightPosition;
//layout(location = 8) uniform vec3 pntLightColor;
//layout(location = 9) uniform vec3 attenuation; 

// Camera
layout(location = 10) uniform vec3 camPosition;

// Mesh Properties
layout(location = 11) uniform vec3 reflection; // .x (ambient) .y (diffuse) .z (specular)
layout(location = 12) uniform float glossiness;

// Texture
layout(location = 13) uniform sampler2D tex;

out vec4 fragColor;

float calcIntensity(float value) {
	if (value > 0.8) return 1.0;
	if (value > 0.3) return 0.8;
	if (value > 0.0) return 0.6;
	return 0.4;
}

vec3[2] calcPointLight(PointLight light, vec3 vNormal, vec3 viewDir) {
	float d = distance(fragPosition, light.position);
	float a = light.attenuation.x + (light.attenuation.y * d) + (light.attenuation.z * (d * d));

	float TORCH_LIGHT_MULTIPLIER = 5;

	vec3 lightDir = normalize(light.position - fragPosition);
	float intensity = smoothstep(0, 0.01, max(dot(vNormal, lightDir), 0.0)) * TORCH_LIGHT_MULTIPLIER;
	vec3 diffuse = (intensity * light.color) / a;

	vec3 reflDirection = reflect(-lightDir, vNormal);
	float reflDot = max(dot(viewDir, reflDirection) * intensity, 0.0);
	float reflIntensity = smoothstep(0, 0.01, pow(reflDot, glossiness * glossiness));
	vec3 specular = (reflIntensity * light.color) / a;

	vec3 result[2];
	result[0] = diffuse;
	result[1] = specular;
	return result;
}

void main() {
	vec3 vertNormal = normalize(vertexNormal);
	vec3 viewDirection = normalize(camPosition - fragPosition);

	// Ambient
	vec3 ambient = reflection.x * vec3(1.0f);

	// Diffuse (Directional)
	vec3 dLightDir = normalize(-dirLightDirection);
	float dIntensity = smoothstep(0, 0.01, max(dot(vertNormal, dLightDir), 0.0));
	//float dLightFactor = calcIntensity(dIntensity);
	vec3 dDiffuse = dIntensity * dirLightColor;

	vec3 diffuse = reflection.y * dDiffuse;

	// Specular (Directional)
	vec3 dReflDirection = reflect(-dLightDir, vertNormal);
	float dReflDot = max(dot(viewDirection, dReflDirection) * dIntensity, 0.0);
	float dReflIntensity = smoothstep(0, 0.01, pow(dReflDot, glossiness * glossiness));
	vec3 dSpecular = (dReflIntensity * dirLightColor);

	vec3 specular = reflection.z * dSpecular;

	vec3 result = (ambient + diffuse) * texture(tex, uvCoord).rgb + specular;

	for (int i = 0; i < pointLights.length(); i++) {
		PointLight light = pointLights[i];
		vec3[2] point = calcPointLight(light, vertNormal, viewDirection);
		result += (point[0]) * texture(tex, uvCoord).rgb + point[1];
	}
	
	// Diffuse (Point)
	//vec3 pLightDir = normalize(pntLightPosition - fragPosition);
	//float pIntensity = smoothstep(0, 0.01, dot(vertNormal, pLightDir));
	//float pLightFactor = calcIntensity(pIntensity);
	//vec3 pDiffuse = pLightFactor * pntLightColor / a;

	// Specular (Point)
	//vec3 pReflDirection = reflect(-pLightDir, vertNormal);
	//float pReflDot = max(dot(viewDirection, pReflDirection) * pIntensity, 0.0);
	//float pReflIntensity = smoothstep(0, 0.1, pow(pReflDot, glossiness * glossiness));
	//vec3 pSpecular = (pReflIntensity * pntLightColor) / a;

	// Specular (Directional)
	//vec3 dReflDirection = reflect(-dLightDir, vertNormal);
	//float dReflDot = max(dot(viewDirection, dReflDirection) * dIntensity, 0.0);
	//float dReflIntensity = smoothstep(0, 0.1, pow(dReflDot, glossiness * glossiness));
	//vec3 dSpecular = (dReflIntensity * dirLightColor);

	//vec3 specular = reflection.z * dSpecular;

	// Vignette (for later)
	//vec2 dims = vec2(1280, 1024);
	//vec2 center_pos = (gl_FragCoord.xy / dims.xy) - vec2(0.5);
	//center_pos.x *= dims.x / dims.y;
	//float len = length(center_pos);
	//float vig = smoothstep(0.75, 0.3, len);

	fragColor = vec4(result, 1.0);
}