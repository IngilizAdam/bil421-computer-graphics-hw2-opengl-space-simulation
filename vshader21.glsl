#version 150

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;
out vec4 color;

uniform mat4 model;
uniform mat4 view_projection;

void main() {
	gl_Position = view_projection * model * vPosition;
	color = vColor;
}
