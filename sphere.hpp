
#ifndef SPHERE_HPP_
#define SPHERE_HPP_

#include "common.hpp"

class Sphere{

public:
	std::vector<glm::vec3> calculateSpherePoints(int subdivisions);
	std::vector<glm::vec3> calculateNormals();
	std::vector<unsigned int> getIndices();

private:
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
	glm::vec3 scale(glm::vec3 v);
	std::vector<unsigned int> subdivide(std::vector<unsigned int> points);
	std::vector<unsigned int> icosahedron();
};




#endif /* SPHERE_HPP_ */
