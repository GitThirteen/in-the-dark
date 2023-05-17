#version 430

in vec3 fragPosition;
in vec3 vertexNormal;
//in vec3 uvCoord;
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

void main() {
	// Setup
	float d = distance(fragPosition, pntLightPosition);
	float a = attenuation.x + (attenuation.y * d) + (attenuation.z * (d * d));

	vec3 vertNormal = normalize(vertexNormal);
	vec3 viewDirection = normalize(camPosition - fragPosition);

	// Ambient
	vec3 ambient = reflection.x * vec3(1.0f);

	// Diffuse (Point)
	vec3 pLightDir = normalize(pntLightPosition - fragPosition);
	float pDot = max(dot(vertNormal, pLightDir), 0.0);
	vec3 pDiffuse = (pDot * pntLightColor) / a;
	
	// Diffuse (Directional)
	vec3 dLightDir = normalize(-dirLightDirection);
	float dDot = max(dot(vertNormal, dLightDir), 0.0);
	vec3 dDiffuse = (dDot * dirLightColor);

	vec3 diffuse = reflection.y * (pDiffuse + dDiffuse);

	// Specular (Point)
	vec3 pReflDirection = reflect(-pLightDir, vertNormal);
	float pReflDot = pow(max(dot(viewDirection, pReflDirection), 0.0), glossiness);
	vec3 pSpecular = (pReflDot * pntLightColor) / a;

	// Specular (Directional)
	vec3 dReflDirection = reflect(-dLightDir, vertNormal);
	float dReflDot = pow(max(dot(viewDirection, dReflDirection), 0.0), glossiness);
	vec3 dSpecular = (dReflDot * dirLightColor);

	vec3 specular = reflection.z * (pSpecular + dSpecular);

	vec3 combined = (ambient + diffuse) * vertexColor.rgb + specular;
	fragColor = vec4(combined, 1.0);
}