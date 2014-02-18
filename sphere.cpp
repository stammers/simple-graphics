#include "sphere.hpp"
#include <algorithm>



glm::vec3 Sphere::scale(glm::vec3 v){
	GLfloat x = v.x;
	GLfloat y = v.y;
	GLfloat z = v.z;

	GLfloat r = 1.0f/sqrt(x*x + y*y + z*z);

	return glm::vec3(x*r, y*r, z*r);
}

std::vector<unsigned int> Sphere::subdivide(std::vector<unsigned int> points){
	std::vector<unsigned int> newPoints;
	int size = points.size() / 3;

	for(int i = 0; i < size; i++){
		//done backwards
		unsigned int v1 = points.back();
		points.pop_back();
		unsigned int v2 = points.back();
		points.pop_back();
		unsigned int v3 = points.back();
		points.pop_back();


		glm::vec3 v4 = vertices[v1] + vertices[v2];
		v4 = scale(v4);


		//find out if vertex is already added
		//if it is get the position otherwise add it
		//done to get rid of all duplicate vertices, reduced number by a half by doing this
		//decided that as this is only done once versus the vertices could be drawn multiple
		//times, the time taken to do this was beneficial as it drastically reduced the number of 
		//vertices to be passed.
		unsigned int position = std::find(vertices.begin(), vertices.end(), v4) - vertices.begin();
		unsigned int v4Position;
		if(position < vertices.size()) {
			v4Position = position ;
		}else{
			vertices.push_back(v4);
			v4Position = vertices.size()-1;
		}



		glm::vec3 v5 = vertices[v2] + vertices[v3];
		v5 = scale(v5);



		position = std::find(vertices.begin(), vertices.end(), v5) - vertices.begin();
		unsigned int v5Position;
		if(position < vertices.size()) {
			v5Position = position ;
		}else{
			vertices.push_back(v5);
			v5Position = vertices.size()-1;
		}

		glm::vec3 v6 = vertices[v3] + vertices[v1];
		v6 = scale(v6);



		position = std::find(vertices.begin(), vertices.end(), v6) - vertices.begin();
		unsigned int v6Position;
		if(position < vertices.size()) {
			v6Position = position ;
		}else{
			vertices.push_back(v6);
			v6Position = vertices.size()-1;
		}

		newPoints.push_back(v1);

		newPoints.push_back(v4Position);

		newPoints.push_back(v6Position);

		newPoints.push_back(v4Position);
		newPoints.push_back(v2);
		newPoints.push_back(v5Position);

		newPoints.push_back(v6Position);
		newPoints.push_back(v4Position);
		newPoints.push_back(v5Position);

		newPoints.push_back(v6Position);
		newPoints.push_back(v5Position);
		newPoints.push_back(v3);
	}

	return newPoints;
}

//creates an icosahedron whose faces can be subdivided to to approximate a sphere
std::vector<unsigned int> Sphere::icosahedron(){
	//converted into radians
	double theta = 26.565 * M_PI / 180.0; //value obtained from http://www.fho-emden.de/~hoffmann/ikos27042002.pdf

	double sth = std::sin(theta);
	double cth = std::cos(theta);


	vertices.push_back(glm::vec3(0.0f, 0.0f, -1.0f)); // the front vertex

	// the lower half
	double psi = M_PI / 5.0;
	for (int i = 1; i < 6; i++) {
		vertices.push_back(glm::vec3(cth * std::cos(psi), cth * std::sin(psi), -sth));
		psi += 2.0 * M_PI / 5.0;
	}

	// the upper half
	psi = 0.0;
	for (int i = 6; i < 11; i++) {
		vertices.push_back(glm::vec3(cth * std::cos(psi), cth * std::sin(psi), sth));
		psi += 2.0 * M_PI / 5.0;
	}

	vertices.push_back(glm::vec3(0.0f, 0.0f, 1.0f)); // the back vertex

	std::vector<unsigned int> indices;

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(1);

	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(2);

	indices.push_back(0);
	indices.push_back(4);
	indices.push_back(3);

	indices.push_back(0);
	indices.push_back(5);
	indices.push_back(4);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(5);

	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(7);

	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(8);

	indices.push_back(3);
	indices.push_back(4);
	indices.push_back(9);

	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(10);

	indices.push_back(5);
	indices.push_back(1);
	indices.push_back(6);

	indices.push_back(1);
	indices.push_back(7);
	indices.push_back(6);

	indices.push_back(2);
	indices.push_back(8);
	indices.push_back(7);

	indices.push_back(3);
	indices.push_back(9);
	indices.push_back(8);

	indices.push_back(4);
	indices.push_back(10);
	indices.push_back(9);

	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(10);

	indices.push_back(6);
	indices.push_back(7);
	indices.push_back(11);

	indices.push_back(7);
	indices.push_back(8);
	indices.push_back(11);

	indices.push_back(8);
	indices.push_back(9);
	indices.push_back(11);

	indices.push_back(9);
	indices.push_back(10);
	indices.push_back(11);

	indices.push_back(10);
	indices.push_back(6);
	indices.push_back(11);

	return indices;
}

std::vector<glm::vec3> Sphere::calculateSpherePoints(int subdivisions){
	indices = icosahedron();

	for(int i = 0; i < subdivisions; i++){
		indices = subdivide(indices);
	}

	return vertices;

}

std::vector<unsigned int> Sphere::getIndices(){
	return indices;
}

std::vector<glm::vec3> Sphere::calculateNormals(){
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> pointsTemp = vertices;
	int size = pointsTemp.size();
	for(int i = 0; i < size; i++){
		GLfloat x = pointsTemp.back().x;
		GLfloat y = pointsTemp.back().y;
		GLfloat z = pointsTemp.back().z;
		pointsTemp.pop_back();

		//as the normal at a vertex is a line through the
		//vertex and the centre of the sphere
		//the normal is normalise(P-C) where P is the vertex
		//point and C is the sphere centre.
		//can draw lines to show these normals as lines
		//from the vertex to a point on a slightly larger sphere
		//with the same centre
		GLfloat r = 1.2f/sqrt(x*x + y*y + z*z);

		normals.push_back(glm::vec3(x, y, z));
		normals.push_back(glm::vec3(x*r, y*r, z*r));
	}

	return normals;
}


