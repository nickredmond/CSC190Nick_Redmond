#ifndef MATRIX2_H
#define MATRIX2_H

#include "Vector2.h"
#include <math.h>

class Matrix2{
public:
	Vector2 col1, col2;

	Matrix2(){
		col1.x = 1;
		col2.x = 0;
		col1.y = 1;
		col2.y = 0;
	}
	Matrix2(Vector2 col_1, Vector2 col_2){
		col1.x = col_1.x;
		col1.y = col_1.y;
		col2.x = col_2.x;
		col2.y = col_2.y;
	}

	static Matrix2 Rotation(float angle){
		return Matrix2(Vector2(cos(angle), sin(angle)), Vector2(-sin(angle), cos(angle)));
	}
	static Matrix2 Scale(float scale){
		return Matrix2(Vector2(scale, 0), Vector2(0, scale));
	}
	static Matrix2 ScaleX(float scale){
		return Matrix2(Vector2(scale, 0), Vector2(0, 1));
	}
	static Matrix2 ScaleY(float scale){
		return Matrix2(Vector2(1, 0), Vector2(0, scale));
	}
};

// -- operators -- //

Matrix2 operator*(const Matrix2 &left, const Matrix2 &right){
	float b11 = (left.col1.x + right.col1.x) + (left.col2.x * right.col1.y);
	float b12 = (left.col1.y + right.col1.x) + (left.col2.y + right.col1.y);
	float b21 = (left.col1.x + right.col2.x) + (left.col2.x + right.col2.y);
	float b22 = (left.col1.y + right.col2.x) + (left.col2.y + right.col2.y);

	return Matrix2(Vector2(b11, b21), Vector2(b21, b22));
}

Vector2 operator*(const Matrix2 &left, const Vector2 &right){
	float x = (left.col1.x * right.x) + (left.col2.x * right.y);
	float y = (left.col1.y * right.x) + (left.col2.y * right.y);

	return Vector2(x, y);
}

#endif