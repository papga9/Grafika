#version 400 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor;

uniform mat4 trans;
	
out vec3 ourColor;

void main() {
    gl_Position = trans * vec4(aPos, 1.0);
    ourColor = aColor; // set ourColor to the input color we got from the vertex data
}