#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

struct Vector2{
public:
	float x, y;

	Vector2(float xVal = 0.0f, float yVal = 0.0f) : x(x), y(y){
		x = xVal;
		y = yVal;
	}

	float dotProduct(const Vector2 &right){
		float result = (x * right.x) + (y * right.y);
		return result;
	}

	float length(){
		return sqrt((x * x) + (y * y));
	}

	float lengthSquared(){
		return (x * x) + (y * y);
	}

	Vector2 normalized(){
		float len = length();

		Vector2 result;
		result.x = x / len;
		result.y = y / len;

		return result;
	}

	Vector2 perpCW(){
		return Vector2(y, -x);
	}

	Vector2 perpCCW(){
		return Vector2(-y,x);
	}

	float* operator*(){return &x;}
};

//-- operators --//

Vector2 operator+(const Vector2 &left, const Vector2 &right){
	Vector2 result;
	result.x = left.x + right.x;
	result.y = left.y + right.y;

	return result;
}

Vector2 operator-(const Vector2 &left, const Vector2 &right){
	Vector2 result;
	result.x = left.x - right.x;
	result.y = left.y - right.y;

	return result;
}

Vector2 operator*(const float &scalar, const Vector2 &right){
	Vector2 result;
	result.x = scalar * right.x;
	result.y = scalar * right.y;

	return result;
}

Vector2 operator*(const Vector2 &left, const float &scalar){
	Vector2 result;
	result.x = left.x * scalar;
	result.y = left.y * scalar;

	return result;
}

Vector2 operator/(const Vector2 &left, const float scalar){
	Vector2 result;
	result.x = left.x / scalar;
	result.y = left.y / scalar;
	
	return result;
}

//std::ostream& operator<<(std::ostream &stream, const Vector2 vec){
//	cout << "{ " << vec.x << ", " << vec.y << " }" << endl;
//	return stream;
//}

//-- functions --//

Vector2 LERP(Vector2 first, Vector2 second, float beta){
	Vector2 result;
	result.x = ((1.0f - beta) * first.x) + (beta * second.x);
	result.y = ((1.0f - beta) * first.y) + (beta * second.y);
	
	return result;
}

float dotProduct(Vector2 &left, Vector2 &right){
	return (left.x * right.x) + (left.y * right.y);
}

float distance(Vector2 &v1, Vector2 &v2){
	float discriminant = pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2);
	return sqrt(discriminant);
}

#endif