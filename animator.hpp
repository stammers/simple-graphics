#ifndef ANIMATOR_HPP_
#define ANIMATOR_HPP_

#include "common.hpp"

class Animator{

public:
	Animator(GLfloat startX, GLfloat startY);
	void animate(double time);
	GLfloat getX();
	GLfloat getY();


private:
	GLfloat movementX;
	GLfloat movementY;

};



#endif /* ANIMATOR_HPP_ */
