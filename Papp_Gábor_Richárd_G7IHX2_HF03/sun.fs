#version 400 core
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;
out vec4 fragColor;

uniform sampler2D texture1;
uniform vec3 color;

void main() {
	fragColor = texture(texture1, texCoord) * vec4(color, 1.0f);
}
