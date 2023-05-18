#version 430

in vec3 fragPosition;
in vec3 vertexNormal;
//in vec2 uvCoord;
in vec4 vertexColor;

// Directional Light
layout(location = 5) uniform vec3 dirLightDirection;
layout(location = 6) uniform vec3 dirLightColor;

// Point Light
layout(location = 7) uniform vec3 pntLightPosition;
layout(location = 8) uniform vec3 pntLightColor;
layout(location = 9) uniform vec3 attenuation; // .x (constant) .y (linear) .z (quadratic)

// Camera
layout(location = 10) uniform vec3 camPosition;

// Mesh Properties
layout(location = 11) uniform vec3 reflection; // .x (ambient) .y (diffuse) .z (specular)
layout(location = 12) uniform float glossiness;

out vec4 fragColor;

float calcIntensity(float value) {
	if (value > 0.8) return 1.0;
	if (value > 0.3) return 0.8;
	if (value > 0.0) return 0.6;
	return 0.3;
}

void main() {
	float d = distance(fragPosition, pntLightPosition);
	float a = attenuation.x + (attenuation.y * d) + (attenuation.z * (d * d));

	vec3 vertNormal = normalize(vertexNormal);
	vec3 viewDirection = normalize(camPosition - fragPosition);

	// Ambient
	vec3 ambient = reflection.x * vec3(1.0f);

	// Diffuse (Point)
	vec3 pLightDir = normalize(pntLightPosition - fragPosition);
	float pIntensity = smoothstep(0, 0.01, dot(vertNormal, pLightDir));
	float pLightFactor = calcIntensity(pIntensity);
	vec3 pDiffuse = pLightFactor * pntLightColor / a;
	
	// Diffuse (Directional)
	vec3 dLightDir = normalize(-dirLightDirection);
	float dIntensity = smoothstep(0, 0.01, dot(vertNormal, dLightDir));
	float dLightFactor = calcIntensity(dIntensity);
	vec3 dDiffuse = dLightFactor * dirLightColor;

	vec3 diffuse = reflection.y * (pDiffuse + dDiffuse);

	// Specular (Point)
	vec3 pReflDirection = reflect(-pLightDir, vertNormal);
	float pReflDot = max(dot(viewDirection, pReflDirection) * pIntensity, 0.0);
	float pReflIntensity = smoothstep(0, 0.1, pow(pReflDot, glossiness * glossiness));
	vec3 pSpecular = (pReflIntensity * pntLightColor) / a;

	// Specular (Directional)
	vec3 dReflDirection = reflect(-dLightDir, vertNormal);
	float dReflDot = max(dot(viewDirection, dReflDirection) * dIntensity, 0.0);
	float dReflIntensity = smoothstep(0, 0.1, pow(dReflDot, glossiness * glossiness));
	vec3 dSpecular = (dReflIntensity * dirLightColor);

	vec3 specular = reflection.z * (pSpecular + dSpecular);

	// Vignette (for later)
	//vec2 dims = vec2(1280, 1024);
	//vec2 center_pos = (gl_FragCoord.xy / dims.xy) - vec2(0.5);
	//center_pos.x *= dims.x / dims.y;
	//float len = length(center_pos);
	//float vig = smoothstep(0.75, 0.3, len);

	vec3 combined = (ambient + diffuse) * vertexColor.rgb + specular;
	fragColor = vec4(combined, 1.0);
}