#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoord;

out vec2 TexCoord;

void main() {
	gl_Position = vec4(inPosition.x, inPosition.y, inPosition.z, 1.0);
	TexCoord = inTexCoord;
}
