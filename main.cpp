#include "utils.hpp"
#include "renderer.hpp"
#include "animator.hpp"

static double startTime;

int scene = 1;

//angles to allow rotation with the arrow keys
float angleX = 0.0; //angle around x axis
float angleY = 0.0; //angle around y axis

int subdivisions = 3;
int circleTriangles = 20;

Animator sphereAnimator (-1.0f, -1.0f);
Animator coneAnimator (1.0f, 1.0f);

Renderer render;

GLuint cubeVAO;
GLuint sphereWireframeVAO;
GLuint normalsVAO;
GLuint coneVAO;
GLuint shadedSphereVAO;

//compiled shaders
GLuint shaderProgramShadedColour;
GLuint shaderProgramBasicColour;
GLuint shaderProgramNormals;

//changes the scene to render based on the key pressed
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GL_TRUE);
	}else if(key == GLFW_KEY_A && action == GLFW_PRESS){
		scene = 1;
		angleX = 0;
		angleY = 0;
		//only creates the sphere if there isn't a reference to the VAO already containing it
		if(sphereWireframeVAO == 0){
			sphereWireframeVAO = render.sphere(subdivisions);
		}
	}else if(key == GLFW_KEY_B && action == GLFW_PRESS){
		scene = 2;
		angleX = 30;
		angleY = 0;
		if(coneVAO == 0){
			coneVAO = render.cone(circleTriangles);
		}
	}else if(key == GLFW_KEY_C && action == GLFW_PRESS){
		scene = 3;
		angleX = 0;
		angleY = 0;
		if(sphereWireframeVAO == 0){
			sphereWireframeVAO = render.sphere(subdivisions);
		}
		if(normalsVAO == 0){
			normalsVAO = render.sphereNormals(subdivisions);
		}
	}else if(key == GLFW_KEY_D && action == GLFW_PRESS){
		scene = 4;
		angleX = 0;
		angleY = 0;
		if(shadedSphereVAO == 0){
			shadedSphereVAO = render.shadedSphere(subdivisions);
		}
	}else if(key == GLFW_KEY_E && action == GLFW_PRESS){
		scene = 5;
		angleX = 0;
		angleY = 0;
		if(coneVAO == 0){
			coneVAO = render.cone(circleTriangles);
		}
		if(sphereWireframeVAO == 0){
			sphereWireframeVAO = render.sphere(subdivisions);
		}
		startTime  =  glfwGetTime(); // Set the initial time to now
	}else if(key == GLFW_KEY_LEFT){
		angleY -=5;
		if(angleY < 0){
			angleY = 360+angleY;
		}
	}else if(key == GLFW_KEY_RIGHT){
		angleY +=5;
		if(angleY > 359){
			angleY = 0+(angleY-359);
		}
	}else if(key == GLFW_KEY_UP){
		angleX +=5;
		if(angleX > 359){
			angleX = 0+(angleX-359);
		}
	}else if(key == GLFW_KEY_DOWN){
		angleX -=5;
		if(angleX < 0){
			angleX = 360 + angleX;
		}
	}
}

void setupShaders(){
	// Create and compile our GLSL program from the shaders
	shaderProgramShadedColour = LoadShaders( "vertexShadedColour.vert", "fragmentShadedColour.frag" );
	shaderProgramBasicColour = LoadShaders("vertexBasicColour.vert", "fragmentBasicColour.frag");
	shaderProgramNormals = LoadShaders("vertexBasicColour.vert", "fragmentBasicColourNormals.frag");
}

void mvp(GLfloat offsetX, GLfloat offsetY, GLuint shaderProgram){
	// Projection matrix : 45° Field of View, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);

	// Camera matrix
	glm::mat4 View = glm::mat4(1.);
	View = glm::translate(View, glm::vec3(0.f, 0.f, -6.0f));

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0);

	Model = glm::translate(Model, glm::vec3(offsetX, offsetY, 0.0f));

	//allows the model to be rotated around the x and y axis
	Model = glm::rotate(Model, angleX, glm::vec3(1.0, 0.0, 0.0));
	Model = glm::rotate(Model, angleY, glm::vec3(0.0, 1.0, 0.0));

	glm::mat4 MVP = Projection * View * Model;

	// Get a handle for our "MVP" uniform.
	GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");

	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
}

//sends the light position and colour to the currently bound shader
void lighting(){
	glm::vec3 lightPosition = glm::normalize(glm::vec3(0.0f, 0.0f, 6.0f));
	GLuint LightID = glGetUniformLocation(shaderProgramShadedColour, "lightDirection");
	glUniform3f(LightID, lightPosition.x, lightPosition.y, lightPosition.z);

	glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f);
	GLuint ColourID = glGetUniformLocation(shaderProgramShadedColour, "lightColour");
	glUniform3f(ColourID, colour.r, colour.g, colour.b);
}


void renderScene(){

	int multiplierSphereVertices = 1;
	multiplierSphereVertices = 1;
	int normalsNumber = 12;

	//calculates the number of vertices on the sphere
	for(int i = 0; i < subdivisions; i++){
		multiplierSphereVertices = multiplierSphereVertices * 4;
	}
	multiplierSphereVertices = 20 * 3 * multiplierSphereVertices;


	switch(scene){
	case 1:

		glUseProgram (shaderProgramBasicColour);

		glBindVertexArray (sphereWireframeVAO);
		mvp(0.0f, 0.0f, shaderProgramBasicColour);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, multiplierSphereVertices, GL_UNSIGNED_INT, (void*)0);
		glClearColor(0., 0., 0., 1.);
		break;

	case 2:
		glUseProgram (shaderProgramBasicColour);

		glBindVertexArray (coneVAO);
		mvp(0.0f, 0.0f, shaderProgramBasicColour);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, circleTriangles * 3 * 2, GL_UNSIGNED_INT, (void*)0);
		glClearColor(0., 0., 0., 1.);
		break;

	case 3:
		glUseProgram (shaderProgramBasicColour);
		mvp( 0.0f, 0.0f, shaderProgramBasicColour);

		glBindVertexArray (sphereWireframeVAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, multiplierSphereVertices, GL_UNSIGNED_INT, (void*)0);

		//calculates the number of normals to draw
		for(int i = 0; i <= subdivisions; i++){
			if(i > 0){
				normalsNumber = normalsNumber + (30 * pow(4, (i-1)));
			}
		}

		glUseProgram (shaderProgramNormals);
		mvp( 0.0f, 0.0f, shaderProgramNormals);
		glBindVertexArray (normalsVAO);
		glDrawArrays(GL_LINES, 0, normalsNumber * 2);

		glClearColor(0., 0., 0., 1.);
		break;
	case 4:
		glUseProgram (shaderProgramShadedColour);

		glBindVertexArray (shadedSphereVAO);
		mvp(0.0f, 0.0f, shaderProgramShadedColour);
		lighting();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, multiplierSphereVertices, GL_UNSIGNED_INT, (void*)0);
		glClearColor(0., 0., 0., 1.);
		break;
	case 5:

		//ensures the animation is independent of the Framerate
		double duration = glfwGetTime() - startTime;
		coneAnimator.animate(duration);
		sphereAnimator.animate(duration);
		startTime = glfwGetTime();

		glUseProgram (shaderProgramBasicColour);
		glBindVertexArray (coneVAO);
		mvp(coneAnimator.getX(), coneAnimator.getY(), shaderProgramBasicColour);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, circleTriangles * 3 * 2, GL_UNSIGNED_INT, (void*)0);

		glBindVertexArray (sphereWireframeVAO);
		mvp(sphereAnimator.getX(), sphereAnimator.getY(), shaderProgramBasicColour);
		glDrawElements(GL_TRIANGLES, multiplierSphereVertices, GL_UNSIGNED_INT, (void*)0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


		glClearColor(0., 0., 0., 1.);
		break;

	}
}

int main( void ) {

	// Initialize GLFW
	if( !glfwInit() ) {
		exit( EXIT_FAILURE );
	}

	// Open an OpenGL window
	GLFWwindow* window;

	// Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(640, 640, "COMP3004", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	int err = glewInit();
	if (GLEW_OK != err) {
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	setupShaders();
	glfwSetKeyCallback(window, key_callback);

	glDepthMask( GL_TRUE );
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	//creates the sphere VAO so that it can be displayed on screen
	sphereWireframeVAO = render.sphere(subdivisions);

	// Main loop
	while( !glfwWindowShouldClose(window) ) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		renderScene();
		// Swap front and back rendering buffers
		glfwSwapBuffers(window);

		//Poll for and process events
		glfwPollEvents();

	}
	glfwDestroyWindow(window);

	glfwTerminate();
	exit( EXIT_SUCCESS );
}







