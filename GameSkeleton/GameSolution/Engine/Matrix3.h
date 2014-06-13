#ifndef MATRIX3_H
#define MATRIX3_H

#include "Vector2.h"
#include <math.h>

class Matrix3{
public:
	float data[9];

	Matrix3(){
		data[0] = 1;
		data[1] = 0;
		data[2] = 0;
		data[3] = 0;
		data[4] = 1;
		data[5] = 0;
		data[6] = 0;
		data[7] = 0;
		data[8] = 1;
	}

	Matrix3(Vector2 col_1, Vector2 col_2){
		data[0] = col_1.x;
		data[1] = col_2.x;
		data[2] = 0;
		data[3] = col_1.y;
		data[4] = col_2.y;
		data[5] = 0;
		data[6] = 0;
		data[7] = 0;
		data[8] = 1;
	}

	static Matrix3 Rotation(float angle){
		return Matrix3(Vector2(cos(angle), sin(angle)), Vector2(-sin(angle), cos(angle)));
	}
	static Matrix3 Scale(float scale){
		return Matrix3(Vector2(scale, 0), Vector2(0, scale));
	}
	static Matrix3 ScaleX(float scale){
		return Matrix3(Vector2(scale, 0), Vector2(0, 1));
	}
	static Matrix3 ScaleY(float scale){
		return Matrix3(Vector2(1, 0), Vector2(0, scale));
	}
	static Matrix3 Translation(float x, float y){
		Matrix3 matrix = Matrix3(Vector2(1, 0), Vector2(0, 1));
		matrix.data[2] = x; //col3 = Vector2(x, y);
		matrix.data[5] = y;
		return matrix;
	}
	static Matrix3 Translation(Vector2& t){
		Matrix3 matrix = Matrix3(Vector2(1, 0), Vector2(0, 1));
		//matrix.col3.x = t.x;
		//matrix.col3.y = t.y;
		matrix.data[2] = t.x;
		matrix.data[5] = t.y;

		return matrix;
	}

	Matrix3 Translate(float x, float y){
		Matrix3 matrix = Matrix3(Vector2(data[0], data[3]), Vector2(data[1], data[4]));
		matrix.data[2] = data[2] + x;
		matrix.data[5] = data[5] + y;

		return matrix;
	}
	Matrix3 Translate(Vector2& t){
		Matrix3 matrix = Matrix3(Vector2(data[0], data[3]), Vector2(data[1], data[4]));
		matrix.data[2] += t.x;
		matrix.data[5] += t.y;

		return matrix;
	}
};

// -- operators -- //

Matrix3 operator*(const Matrix3 &left, const Matrix3 &right){
	float b11 = (left.data[0] * right.data[0]) + (left.data[1] * right.data[3]);
	float b12 = (left.data[3] * right.data[0]) + (left.data[4] * right.data[3]);
	float b21 = (left.data[0] * right.data[1]) + (left.data[1] * right.data[4]);
	float b22 = (left.data[3] * right.data[1]) + (left.data[4] * right.data[4]);

	Matrix3 result = Matrix3(Vector2(b11, b21), Vector2(b12, b22));
	result.data[2] = (left.data[0] * right.data[2]) + (left.data[1] * right.data[5]) + left.data[2];
	result.data[5] = (left.data[3] * right.data[2]) + (left.data[4] * right.data[5]) + left.data[5];

	return result;
}

Vector2 operator*(const Matrix3 &left, const Vector2 &right){
	float x = (left.data[0] * right.x) + (left.data[1] * right.y) + left.data[2];
	float y = (left.data[3] * right.x) + (left.data[4] * right.y) + left.data[5];

	return Vector2(x, y);
}

#endif