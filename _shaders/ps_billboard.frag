#version 430

in vec2 vertexUV;

layout(location = 13) uniform sampler2D tex;

out vec4 FragColor;

void main() {
    FragColor = texture2D(tex, vertexUV);

    //if (FragColor.r >= 0.9 && FragColor.g >= 0.9 && FragColor.b >= 0.9) {
    //    discard;
    //}
}