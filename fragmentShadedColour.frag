#version 330 core
in vec3 normal;

uniform vec3 lightDirection;
uniform vec3 lightColour;

out vec4 frag_colour;

//uses ambient and difuse lighting
void main () {
	float theta = dot(normal, lightDirection);
	vec3 ambient = vec3(0.1, 0.1, 0.1) * lightColour;
	vec3 colour = ambient + lightColour * clamp(theta, 0.0, 1.0);
	frag_colour = vec4(colour, 1.0);
}
