#version 430

layout(points) in;
layout(points) out;
layout(max_vertices = 60) out;

layout(location = 16) uniform float dt;

in vec3 pPosition[];
in vec3 pVelocity[];
in float pAge[];
in vec3 pColor[];

out vec3 positionOut;
out vec3 velocityOut;
out float ageOut;
out vec3 colorOut;

void main() {
    vec3 posIn = pPosition[0];
    vec3 velIn = pVelocity[0];
    float ageIn = pAge[0];
    vec3 colIn = pColor[0];

    gl_Position = vec4(posIn + velIn * dt, gl_Position.w);
    positionOut = gl_Position.xyz;
    velocityOut = velIn - vec3(0.0, 9.81, 0.0) * dt;
    ageOut = ageIn;
    colorOut = colIn;

    EmitVertex();
    EndPrimitive();
}