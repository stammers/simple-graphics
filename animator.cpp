#include "animator.hpp"

Animator::Animator(GLfloat startX, GLfloat startY){
	movementX = startX;
	movementY = startY;
}

void Animator::animate(double time){
	//moves in a basic square shape
	if(movementY <= -1.0f && movementX < 1.0f){
		movementX += 0.0005f * (time / 0.001);
	}else if(movementY >= 1.0f && movementX > -1.0f){
		movementX -= 0.0005f * (time / 0.001);
	}else if(movementX >= 1.0f && movementY < 1.0f){
		movementY += 0.0005f * (time / 0.001);
	}else if(movementX <= -1.0f && movementY > -1.0f){
		movementY -= 0.0005f * (time / 0.001);
	}
}

GLfloat Animator::getX(){
	return movementX;
}

GLfloat Animator::getY(){
	return movementY;
}


