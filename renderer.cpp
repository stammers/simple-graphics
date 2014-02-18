#include "renderer.hpp"
#include <algorithm>

//returns a VAO reference which is set up to contain one array buffer containing the vertices
GLuint Renderer::setupBuffers(std::vector<glm::vec3> points){

	GLuint vao = 0;
	glGenVertexArrays (1, &vao); //create new vertex array object
	glBindVertexArray (vao); //bind as current vertex array object

	GLuint points_vbo = 0;
	glGenBuffers (1, &points_vbo); // generate empty buffer
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo); //bind as current buffer
	glBufferData (GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), &points[0], GL_STATIC_DRAW); //copy points into buffer

	glEnableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); //defines the layout of our first vertex buffer; "0" means define the layout for attribute number 0. "3" means that the variables are vec3 made from every 3 floats (GL_FLOAT) in the buffer.


	glBindVertexArray(0);
	return vao;
}

//returns a VAO reference which is set up to contain one element buffer containing the indices
//and one array buffer containing the vertices
GLuint Renderer::setupBuffers(std::vector<glm::vec3> points, std::vector<unsigned int> indices){

	GLuint vao = 0;
	glGenVertexArrays (1, &vao); //create new vertex array object
	glBindVertexArray (vao); //bind as current vertex array object

	// Generate a buffer for the indices
	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	GLuint points_vbo = 0;
	glGenBuffers (1, &points_vbo); // generate empty buffer
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo); //bind as current buffer
	glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * points.size(), &points[0], GL_STATIC_DRAW); //copy points into buffer


	glEnableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); //defines the layout of our first vertex buffer; "0" means define the layout for attribute number 0. "3" means that the variables are vec3 made from every 3 floats (GL_FLOAT) in the buffer.

	return vao;
}

GLuint Renderer::sphere(int subdivisions){

	std::vector<glm::vec3> points = sphereObject.calculateSpherePoints(subdivisions);

	std::vector<unsigned int> indices = sphereObject.getIndices();
	return setupBuffers(points, indices);
}

GLuint Renderer::cone(int triangles){

	GLfloat x = 0.0f; //the x centre of the circle
	GLfloat y = -1.0f; //the y centre of the circle
	GLfloat radius = 1.0f; //the radius of the circle
	GLfloat z = 0.0f; //the z centre of the circle


	std::vector<glm::vec3> points;
	points.push_back(glm::vec3(0.0f, 1.0f, 0.0f)); //top point of the cone
	points.push_back(glm::vec3(x, y, z)); //centre of the circle base

	std::vector<unsigned int> indices;

	GLfloat twicePi = 2.0f * M_PI;
	for(int i = 0; i < triangles; i++) {

		//only add this vertex to the vector when i is 0, as it's the start point
		//otherwise each vertex will be added twice
		if(i == 0){
			GLfloat firstX = x + (radius * cos(i *  twicePi / triangles));
			GLfloat firstZ =  z + (radius* sin(i * twicePi / triangles));
			points.push_back(glm::vec3(firstX, y, firstZ));
		}

		GLfloat secondX = x + (radius * cos((i+1) *  twicePi / triangles));
		GLfloat secondZ = z + (radius* sin((i+1) * twicePi / triangles));
		indices.push_back(1);

		unsigned int point1 = points.size()-1;
		indices.push_back(point1);

		if(i+1 < triangles){
			points.push_back(glm::vec3(secondX, y, secondZ));
			unsigned int point2 = points.size()-1;
			indices.push_back(point2);
			indices.push_back(point2);
		}else{
			indices.push_back(2);
			indices.push_back(2);
		}
		indices.push_back(point1);


		indices.push_back(0);
	}

	return setupBuffers(points, indices);
}

GLuint Renderer::sphereNormals(int subdivisions){
	std::vector<glm::vec3> normals = sphereObject.calculateNormals();

	return setupBuffers(normals);

}

GLuint Renderer::shadedSphere(int subdivisions){
	std::vector<glm::vec3> points = sphereObject.calculateSpherePoints(subdivisions);
	std::vector<unsigned int> indices = sphereObject.getIndices();

	/*for each vertex on the sphere the light intensity is calculated
	 *by taking the dot product of the normal at the vertex
	 *and the direction of the light
	 *this is done for each of the red, green and blue components
	 */
	//Now done in vertex shader
//	std::vector<glm::vec3> colours;
//	glm::vec3 lightDirection = glm::normalize(glm::vec3(0, 0, 6));
//	for(unsigned int i = 0; i < points.size(); i++){
//		glm::vec3 v = points[i];
//		glm::vec3 normalised = glm::normalize(v);
//		GLfloat theta = glm::dot(normalised, lightDirection);
//
//		GLfloat red = 1.0f * theta;
//		GLfloat green = 1.0f * theta;
//		GLfloat blue = 1.0f * theta;
//		colours.push_back(glm::vec3(red, green, blue));
//	}

	return setupBuffers(points, indices);
}



