#version 430

layout(points) in;
layout(points) out;
layout(max_vertices = 60) out;

layout(location = 4) uniform float dt;
layout(location = 5) uniform float maxLifetime;
layout(location = 6) uniform vec3 defaultPosition;
layout(location = 7) uniform bool generateParticle;

in vec3 pPosition[];
in vec3 pVelocity[];
in float pAge[];
in vec3 pColor[];

out vec3 positionOut;
out vec3 velocityOut;
out float ageOut;
out vec3 colorOut;

// refer to https://thebookofshaders.com/10/
float random(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    // Iterate over all particles and update them if active
    for (int i = 0; i < gl_in.length(); i++) {
        vec3 position = pPosition[i];
        vec3 velocity = pVelocity[i];
        float age = pAge[i];
        vec3 color = pColor[i];

        age += dt;

        if (age > maxLifetime) continue;

        gl_Position = vec4(position + velocity * dt, gl_Position.w);
        
        positionOut = gl_Position.xyz;
        velocityOut = velocity - vec3(0.0, 9.81, 0.0) * dt * -0.01; // temp, consider replacing with linear velocity (= velocity)
        ageOut = age;
        colorOut = color;

        EmitVertex();
    }

    // Very VERY temporary rng argument that is 100% based on the fact that the frame rate and thus dt
    // isn't locked - this will NOT work if e.g. vsync is enabled, either use random texture or pass random
    // value into shader via uniform every frame @future me
    float rng = random(vec2(fract(dt * 1000), sin(dt * 1000)));

    // Generate new particle if the generateParticle flag is true (elapsedTime > emitInterval on CPU side)
    if (generateParticle) {
        vec3 startPos = defaultPosition;
        //startPos.x += ((rng * 2.0 - 1.0) * 0.25);
        //startPos.z += ((rng * 2.0 - 1.0) * 0.25);
        gl_Position = vec4(startPos, gl_Position.w);

        positionOut = gl_Position.xyz;
        velocityOut = vec3(0.0, 0.0001, 0.0);
        ageOut = 0.0;
        colorOut = vec3(1.0, 0.0, 0.0);

        EmitVertex();
    }

    EndPrimitive();
}