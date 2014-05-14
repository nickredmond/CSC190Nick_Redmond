#include "RenderUI.h"
#include "Engine.h"

#include "Vector2.h"
#include "Matrix2.h"
#include "Matrix3.h"

// vectors //
Vector2 left, right, result;
Vector2 original, normal, cwPerp, ccwPerp;
Vector2 vec1, vec2, projectionVec, rejectionVec;
Vector2 aVector, bVector, aMinusB, aVectorLerp, bVectorLerp, lerpResult;

// matrices //
Vector2 linearResult;

float results[15];

const float span = 0.3f;
Vector2 bottomLeft(-span, -span);
Vector2 topLeft(-span, span);
Vector2 topRight(span, span);
Vector2 bottomRight(span, -span);
Vector2 roofTop(0, span + .25f);

Vector2 lines[] = {
bottomLeft, topLeft,
topLeft, topRight,
topRight, bottomRight,
bottomRight, bottomLeft,
topLeft, roofTop,
topRight, roofTop
};
int numLines = sizeof(lines) / (sizeof(*lines) * 2);

Matrix3 matrices[10];
// float currentTransform[9];
Matrix3 currentTransform;

void basicVectorEqCallback(const BasicVectorEquationInfo& data){
	left = data.scalar1 * Vector2(data.x1, data.y1);
	right = data.scalar2 * Vector2(data.x2, data.y2);

	result = data.add ? (left + right) : (left - right);
}

void perpenDataCallback(const PerpendicularData& data){
	original = Vector2(data.x, data.y);
	normal = original.normalized();
	cwPerp = original.perpCW();
	ccwPerp = original.perpCCW();
}

void dotProductCallback(const DotProductData& data){
	vec1 = Vector2(data.v1i, data.v1j);
	vec2 = Vector2(data.v2i, data.v2j);

	if (data.projectOntoLeftVector){
		float projScalar = dotProduct(vec2, vec1) / vec1.lengthSquared();
		projectionVec = projScalar * vec1;

		rejectionVec = vec2 - projectionVec;
	}
	else{
		float projScalar = dotProduct(vec1, vec2) / vec2.lengthSquared();
		projectionVec = projScalar * vec2;

		rejectionVec = vec1 - projectionVec;
	}
}

void lerpCallback(const LerpData& data){
	aVector = Vector2(data.a_i, data.a_j);
	bVector = Vector2(data.b_i, data.b_j);

	aMinusB = bVector - aVector;
	aVectorLerp = aVector * (1 - data.beta);
	bVectorLerp = bVector * data.beta;

	lerpResult = LERP(aVector, bVector, data.beta);
}

// -- matrices -- //

void matrix2TransformCallback(const MatrixTransformData2D& data){
	Matrix3 left = Matrix3(Vector2(2, 3), Vector2(1, 6));
	Matrix3 right = Matrix3(Vector2(4, 1), Vector2(2, 5));
	Matrix3 test = left * right;
	test;

	Matrix3 transform = Matrix3() * Matrix3::ScaleX(data.scaleX) * Matrix3::ScaleY(data.scaleY) * Matrix3::Rotation(data.rotate) * Matrix3::Translation(data.translateX, data.translateY);
	matrices[data.selectedMatrix] = transform;

	Matrix3 result = Matrix3();

	for (int i = 0; i < 10; i++){
		result = result * matrices[i];
	}

	currentTransform = result;

	//currentTransform[0] = result.col1.x;
	//currentTransform[1] = result.col2.x;
	//currentTransform[2] = result.col3.x;
	//currentTransform[3] = result.col1.y;
	//currentTransform[4] = result.col2.y;
	//currentTransform[5] = result.col3.y;
	//currentTransform[6] = 0;
	//currentTransform[7] = 0;
	//currentTransform[8] = 1;
}

void linearTransCallback(const LinearTransformationData& data){
	Matrix2 matrix = Matrix2(Vector2(data.m00, data.m10), Vector2(data.m01, data.m11));
	Vector2 vec = Vector2(data.v0, data.v1);

	linearResult = matrix * vec;
}

void affineTransCallback(const AffineTransformationData& data){
	// first 9 are matrix, rest are 5 vectors
	const float* matVecData = data.data;
	Vector2 col1 = Vector2(*matVecData, *(matVecData+1));
	Vector2 col2 = Vector2(*(matVecData+3), *(matVecData+4));
	Matrix3 matrix = Matrix3(col1, col2);
	matrix = matrix.Translate(*(matVecData+2), *(matVecData+5));

	for (int i = 0; i < 15; i+=3){
		Vector2 original = Vector2(*(matVecData+i+9), *(matVecData+i+10));
		Vector2 transform = matrix * original;

		results[i] = transform.x;
		results[i+1] = transform.y;
		results[i+2] = 1;
	}

	//int arrIndex = 0;
	//for (int i = 9; i <= 22; i+=3){
	//	results[arrIndex].x = *(matVecData+i);
	//	results[arrIndex].y = *(matVecData+i+1);

	//	arrIndex++;
	//}

	//for (int i = 0; i < 5; i++){
	//	Vector2 transform = matrix * results[i];
	//	results[i].x = transform.x;
	//	results[i].y = transform.y;
	//}
}

int main(int argc, char* argv[])
{
	Engine::Init();

	RenderUI renderUI;

	renderUI.setBasicVectorEquationData(basicVectorEqCallback, &left.x, &right.x, &result.x);
	renderUI.setPerpendicularData(&original.x, &normal.x, &cwPerp.x, &ccwPerp.x, perpenDataCallback);
	renderUI.setDotProductData(&vec1.x, &vec2.x, &projectionVec.x, &rejectionVec.x, dotProductCallback);
	renderUI.setLerpData(&aVector.x, &bVector.x, &aMinusB.x, &aVectorLerp.x, &bVectorLerp.x, &lerpResult.x, lerpCallback);
	renderUI.setLinearTransformationData(&linearResult.x, linearTransCallback);
	renderUI.setAffineTransformationData(&results[0], affineTransCallback);
	renderUI.set2DMatrixVerticesTransformData(&lines[0].x, numLines, &matrices[0].data[0], &currentTransform.data[0], matrix2TransformCallback);

	if( ! renderUI.initialize(argc, argv))
		return -1;
	return renderUI.run();
}