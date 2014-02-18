#version 330 core

layout(location = 0) in vec3 vertex_position;

uniform mat4 MVP;

out vec3 normal;

void main() {
	normal = normalize(vertex_position);
  	gl_Position = MVP * vec4(vertex_position, 1.0);
}