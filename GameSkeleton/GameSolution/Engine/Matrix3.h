#ifndef MATRIX3_H
#define MATRIX3_H

#include "Vector2.h"
#include <math.h>

#include "glm\glm.hpp"

namespace Matrix3
{
	static glm::mat3 Rotation(float angle){
		return glm::mat3(glm::vec3(cos(angle), sin(angle), 0), glm::vec3(-sin(angle), cos(angle), 0), glm::vec3(0, 0, 1));
	}
	static glm::mat3 Scale(float scale){
		return glm::mat3(glm::vec3(scale, 0, 0), glm::vec3(0, scale, 0), glm::vec3(0, 0, 1));
	}
	static glm::mat3 ScaleX(float scale){
		return glm::mat3(glm::vec3(scale, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1));
	}
	static glm::mat3 ScaleY(float scale){
		return glm::mat3(glm::vec3(1, 0, 0), glm::vec3(0, scale, 0), glm::vec3(0, 0, 1));
	}
	static glm::mat3 Translation(float x, float y){
		glm::mat3 matrix = glm::mat3(glm::vec3(1, 0, x), glm::vec3(0, 1, y), glm::vec3(0, 0, 1));
		return matrix;
	}
	static glm::mat3 Translation(glm::vec3& t){
		glm::mat3 matrix = glm::mat3(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(t.x, t.y, 1));
		return matrix;
	}
}
namespace Matrix4
{
	static glm::mat4 RotationX(float angle){
		return glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, cos(angle), sin(angle), 0), glm::vec4(0, -sin(angle), cos(angle), 0), glm::vec4(0, 0, 0, 1));
	}
	static glm::mat4 RotationY(float angle){
		return glm::mat4(glm::vec4(cos(angle), 0, sin(angle), 0), glm::vec4(0, 1, 0, 0), glm::vec4(-sin(angle), 0, cos(angle), 0), glm::vec4(0, 0, 0, 1));
	}
	static glm::mat4 RotationZ(float angle){
		return glm::mat4(glm::vec4(cos(angle), sin(angle), 0, 0), glm::vec4(-sin(angle), cos(angle), 0, 0), glm::vec4(0, 0, 1, 0), glm::vec4(0, 0, 0, 1));
	}

	static glm::mat4 Scale(float scale){
		return glm::mat4(glm::vec4(scale, 0, 0, 0), glm::vec4(0, scale, 0, 0), glm::vec4(0, 0, scale, 0), glm::vec4(0, 0, 0, 1));
	}
	static glm::mat4 ScaleX(float scale){
		return glm::mat4(glm::vec4(scale, 0, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 0, 1, 0), glm::vec4(0, 0, 0, 1));
	}
	static glm::mat4 ScaleY(float scale){
		return glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, scale, 0, 0), glm::vec4(0, 0, 1, 0), glm::vec4(0, 0, 0, 1));
	}
	static glm::mat4 ScaleZ(float scale){
		return glm::mat4(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 0, scale, 0), glm::vec4(0, 0, 0, 1));
	}
	//static glm::mat4 Translation(float x, float y){
	//	glm::mat4 matrix = glm::mat4(glm::vec3(1, 0, x), glm::vec3(0, 1, y), glm::vec3(0, 0, 1));
	//	return matrix;
	//}
	//static glm::mat4 Translation(glm::vec3& t){
	//	glm::mat4 matrix = glm::mat4(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(t.x, t.y, 1));
	//	return matrix;
	//}
}

#endif