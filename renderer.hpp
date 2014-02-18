
#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include "common.hpp"
#include "sphere.hpp"

class Renderer{
public:
	GLuint sphere(int subdivisions);
	GLuint cone(int triangles);
	GLuint sphereNormals(int subdivisions);
	GLuint shadedSphere(int subdivisions);

private:
	Sphere sphereObject;
	GLuint setupBuffers(std::vector<glm::vec3> points);
	GLuint setupBuffers(std::vector<glm::vec3> points, std::vector<unsigned int> indices);
};



#endif /* RENDERER_HPP_ */
