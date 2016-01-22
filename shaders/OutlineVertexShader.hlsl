#version 330 core

in vec3 in_ScreenCoord;
out vec2 ex_TexCoord;

void main(void) {
	ex_TexCoord = (in_ScreenCoord.xy + 1.0f) / 2.0f;
	gl_Position = vec4(in_ScreenCoord, 1.0f);
}