#version 430

layout(points) in;
layout(points) out;
layout(max_vertices = 4) out;

layout(location = 1) uniform mat4 viewProjMat;
layout(location = 2) uniform vec3 camPos;

layout(location = 3) uniform float particleSize;

out vec2 vertexUV;

void main() {
    vec3 pos = gl_in[0].gl_Position.xyz;

    vec3 forward = normalize(camPos - pos);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(forward, up) * particleSize;

    pos -= right;
    gl_Position = viewProjMat * vec4(pos, 1.0);
    vertexUV = vec2(0.0, 0.0);
    EmitVertex();

    pos.y += particleSize;
    gl_Position = viewProjMat * vec4(pos, 1.0);
    vertexUV = vec2(0.0, 1.0);
    EmitVertex();

    pos.y -= particleSize;
    pos += right;
    gl_Position = viewProjMat * vec4(pos, 1.0);
    vertexUV = vec2(1.0, 0.0);
    EmitVertex();

    pos.y += particleSize;
    gl_Position = viewProjMat * vec4(pos, 1.0);
    vertexUV = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();
}