#include "Vector2.h"
#include <math.h>

class Matrix3{
public:
	Vector2 col1;
	float z1;
	Vector2 col2;
	float z2;
	Vector2 col3; // bottom row is always 0,0,1
	float z3;

	Matrix3(){
		col1.x = 1;
		col1.y = 0;
		col2.x = 0;
		col2.y = 1;

		col3.x = 0;
		col3.y = 0;

		z1 = 0;
		z2 = 0;
		z3 = 1;
	}

	Matrix3(Vector2 col_1, Vector2 col_2){
		col1.x = col_1.x;
		col1.y = col_1.y;
		col2.x = col_2.x;
		col2.y = col_2.y;

		col3.x = 0;
		col3.y = 0;

		z1 = 0;
		z2 = 0;
		z3 = 1;
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
		matrix.col3 = Vector2(x, y);
		return matrix;
	}
	static Matrix3 Translation(Vector2& t){
		Matrix3 matrix = Matrix3(Vector2(1, 0), Vector2(0, 1));
		matrix.col3.x = t.x;
		matrix.col3.y = t.y;
		return matrix;
	}

	Matrix3 Translate(float x, float y){
		Matrix3 matrix = Matrix3(Vector2(col1.x, col1.y), Vector2(col2.x, col2.y));
		matrix.col3.x += x;
		matrix.col3.y += y;

		return matrix;
	}
	Matrix3 Translate(Vector2& t){
		Matrix3 matrix = Matrix3(Vector2(col1.x, col1.y), Vector2(col2.x, col2.y));
		matrix.col3.x += t.x;
		matrix.col3.y += t.y;

		return matrix;
	}
};

// -- operators -- //

Matrix3 operator*(const Matrix3 &left, const Matrix3 &right){
	float b11 = (left.col1.x * right.col1.x) + (left.col2.x * right.col1.y);
	float b12 = (left.col1.y * right.col1.x) + (left.col2.y * right.col1.y);
	float b21 = (left.col1.x * right.col2.x) + (left.col2.x * right.col2.y);
	float b22 = (left.col1.y * right.col2.x) + (left.col2.y * right.col2.y);

	Matrix3 result = Matrix3(Vector2(b11, b21), Vector2(b12, b22));
	result.col3.x = (left.col1.x * right.col3.x) + (left.col2.x * right.col3.y) + left.col3.x;
	result.col3.y = (left.col1.y * right.col3.x) + (left.col2.y * right.col3.y) + left.col3.y;
	return result;
}

Vector2 operator*(const Matrix3 &left, const Vector2 &right){
	float x = (left.col1.x * right.x) + (left.col2.x * right.y) + left.col3.x;
	float y = (left.col1.y * right.x) + (left.col2.y * right.y) + left.col3.y;

	return Vector2(x, y);
}