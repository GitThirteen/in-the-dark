#version 430

layout(points) in;
layout(points) out;
layout(max_vertices = 60) out;

layout(location = 16) uniform float dt;
layout(location = 21) uniform float maxLifetime;
layout(location = 22) uniform vec3 defaultPosition;

in vec3 pPosition[];
in vec3 pVelocity[];
in float pAge[];
in vec3 pColor[];

out vec3 positionOut;
out vec3 velocityOut;
out float ageOut;
out vec3 colorOut;

// refer to https://thebookofshaders.com/10/
float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    vec3 position = pPosition[0];
    vec3 velocity = pVelocity[0];
    float age = pAge[0] + dt;
    vec3 color = pColor[0];

    // Small delay between new particles
    if (age < maxLifetime * 0.25) {
        gl_Position = vec4(position + velocity * dt, gl_Position.w);
        positionOut = gl_Position.xyz;
        velocityOut = velocity - vec3(0.0, 9.81, 0.0) * dt * -0.01;
        ageOut = age;
        colorOut = color;

        EmitVertex();
        EndPrimitive();
    }

    // Very VERY temporary rng argument that is 100% based on the fact that the frame rate and thus dt
    // isn't locked, this WILL not work if e.g. vsync is enabled
    float rng = rand(vec2(dt * 10, dt * -5 + 2));

    if (rng < 0.01 || age > maxLifetime * 0.8) { // TODO: RNG should be fps dependent since this is called once per frame
        positionOut.x = defaultPosition.x + ((rng * 2 - 1) * 0.25);
        positionOut.z = defaultPosition.z + ((rng * 2 - 1) * 0.25);
        velocityOut = vec3(0.0f, 0.0001f, 0.0f);
        ageOut = 0.0f;
        colorOut = vec3(1.0f, 0.0f, 0.0f);

        EmitVertex();
        EndPrimitive();
    }

    gl_Position = vec4(position + velocity * dt, gl_Position.w);
    positionOut = gl_Position.xyz;
    velocityOut = velocity - vec3(0.0, 9.81, 0.0) * dt * 0.01;
    ageOut = age;
    colorOut = color;

    //EmitVertex();
    //EndPrimitive();
}