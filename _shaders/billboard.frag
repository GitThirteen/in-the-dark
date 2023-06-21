#version 430

in vec2 vertexUV;

uniform sampler2D colorMap;

out vec4 FragColor;

void main() {
    FragColor = texture2D(colorMap, vertexUV);

    if (FragColor.r >= 0.9 && FragColor.g >= 0.9 && FragColor.b >= 0.9) {
        discard;
    }
}