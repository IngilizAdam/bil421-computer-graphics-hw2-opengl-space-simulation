#version 150

in vec4 vPosition;
in vec4 vColor;
out vec4 color;

uniform mat4 model_view;
uniform mat4 projection;

void main() {
	gl_Position = projection * model_view * vPosition;
	//vec4 tempPosition = projection * model_view * vPosition;
	//gl_Position = tempPosition/tempPosition.w;
	color = vColor;
}
