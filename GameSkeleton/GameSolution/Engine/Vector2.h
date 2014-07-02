#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include "glm\gtc\matrix_transform.hpp"
#include "glm\glm.hpp"

//-- functions --//

glm::vec2 LERP(glm::vec2 first, glm::vec2 second, float beta){
	glm::vec2 result;
	result.x = ((1.0f - beta) * first.x) + (beta * second.x);
	result.y = ((1.0f - beta) * first.y) + (beta * second.y);
	
	return result;
}

float dotProduct(glm::vec2 &left, glm::vec2 &right){
	return (left.x * right.x) + (left.y * right.y);
}

float distance(glm::vec2 &v1, glm::vec2 &v2){
	float discriminant = pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2);
	return sqrt(discriminant);
}

glm::vec2 normalized(glm::vec2 vector){
	float len = vector.length();

	glm::vec2 result;
	result.x = vector.x / len;
	result.y = vector.y / len;

	return result;
}
glm::vec2 perpCW(glm::vec2 vector){
	return glm::vec2(vector.y, -vector.x);
}

glm::vec2 perpCCW(glm::vec2 vector){
	return glm::vec2(-vector.y, vector.x);
}

float lengthSquared(glm::vec2 vector){
	return (vector.x * vector.x) + (vector.y * vector.y);
}

#endif