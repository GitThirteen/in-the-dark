#version 430

in vec2 vertexUV;

layout(location = 4) uniform sampler2D tex;

out vec4 FragColor;

void main() {
    FragColor = texture(tex, vertexUV);

    if (FragColor.r >= 0.9 && FragColor.g >= 0.9 && FragColor.b >= 0.9) {
        discard;
    }
}