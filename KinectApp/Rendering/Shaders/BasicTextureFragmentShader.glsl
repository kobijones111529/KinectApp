#version 330 core

uniform sampler2D uTexture;

in vec2 TexCoord;

out vec4 color;

void main() {
	color = texture(uTexture, TexCoord) * vec4(1.0, 1.0, 1.0, 1.0);
}
