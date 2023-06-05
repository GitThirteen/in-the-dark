#version 430

in vec2 uvCoord;

uniform sampler2D tex;

out vec4 fragColor;

void main() {
	// Vignette (for later)
	//vec2 dims = vec2(1280, 1024);
	//vec2 center_pos = (gl_FragCoord.xy / dims.xy) - vec2(0.5);
	//center_pos.x *= dims.x / dims.y;
	//float len = length(center_pos);
	//float vig = smoothstep(0.75, 0.3, len);

	fragColor = texture(tex, uvCoord);
}