#include "RenderUI.h"
#include "Engine.h"

#include "Vector2.h"
#include "Matrix3.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\detail\func_packing.hpp"

// vectors //
glm::vec2 left, right, result;
glm::vec2 original, normal, cwPerp, ccwPerp;
glm::vec2 vec1, vec2, projectionVec, rejectionVec;
glm::vec2 aVector, bVector, aMinusB, aVectorLerp, bVectorLerp, lerpResult;

// matrices //
glm::vec2 linearResult;

float results[15];

const float span = 0.3f;
glm::vec2 bottomLeft(-span, -span);
glm::vec2 topLeft(-span, span);
glm::vec2 topRight(span, span);
glm::vec2 bottomRight(span, -span);
glm::vec2 roofTop(0, span + .25f);

glm::vec2 lines[] = {
bottomLeft, topLeft,
topLeft, topRight,
topRight, bottomRight,
bottomRight, bottomLeft,
topLeft, roofTop,
topRight, roofTop
};
int numLines = sizeof(lines) / (sizeof(*lines) * 2);

glm::mat3 matrices[10];
glm::mat3 currentTransform;

// 3D //
glm::mat4 matrices_3d[10];
glm::mat4 fullTransform;

void basicVectorEqCallback(const BasicVectorEquationInfo& data){
	left = data.scalar1 * glm::vec2(data.x1, data.y1);
	right = data.scalar2 * glm::vec2(data.x2, data.y2);

	result = data.add ? (left + right) : (left - right);
}

void perpenDataCallback(const PerpendicularData& data){
	original = glm::vec2(data.x, data.y);
	normal = normalized(original);
	cwPerp = perpCW(original);
	ccwPerp = perpCCW(original);
}

void dotProductCallback(const DotProductData& data){
	vec1 = glm::vec2(data.v1i, data.v1j);
	vec2 = glm::vec2(data.v2i, data.v2j);

	if (data.projectOntoLeftVector){
		float projScalar = dotProduct(vec2, vec1) / lengthSquared(vec1);
		projectionVec = projScalar * vec1;

		rejectionVec = vec2 - projectionVec;
	}
	else{
		float projScalar = dotProduct(vec1, vec2) / lengthSquared(vec2);
		projectionVec = projScalar * vec2;

		rejectionVec = vec1 - projectionVec;
	}
}

void lerpCallback(const LerpData& data){
	aVector = glm::vec2(data.a_i, data.a_j);
	bVector = glm::vec2(data.b_i, data.b_j);

	aMinusB = bVector - aVector;
	aVectorLerp = aVector * (1 - data.beta);
	bVectorLerp = bVector * data.beta;

	lerpResult = LERP(aVector, bVector, data.beta);
}
void matrix2TransformCallback(const MatrixTransformData2D& data){
	glm::mat3 transform = glm::mat3() * Matrix3::ScaleX(data.scaleX) * Matrix3::ScaleY(data.scaleY) * Matrix3::Rotation(data.rotate) * Matrix3::Translation(data.translateX, data.translateY);
	matrices[data.selectedMatrix] = transform;

	glm::mat3 result = glm::mat3();

	for (int i = 0; i < 10; i++){
		result = result * matrices[i];
	}

	currentTransform = result;
}

void linearTransCallback(const LinearTransformationData& data){
	glm::mat2 matrix = glm::mat2(glm::vec2(data.m00, data.m10), glm::vec2(data.m01, data.m11));
	glm::vec2 vec = glm::vec2(data.v0, data.v1);

	linearResult = matrix * vec;
}

void affineTransCallback(const AffineTransformationData& data){
	// first 9 are matrix, rest are 5 vectors
	const float* matVecData = data.data;
	glm::vec2 col1 = glm::vec2(*matVecData, *(matVecData+1));
	glm::vec2 col2 = glm::vec2(*(matVecData+3), *(matVecData+4));

	glm::mat3 matrix = glm::mat3(glm::vec3(col1.x, col1.y, 0), glm::vec3(col2.x, col2.y, 0), glm::vec3(0, 0, 1)) * Matrix3::Translation(glm::vec3(*(matVecData+2), *(matVecData+5), 1));

	for (int i = 0; i < 15; i+=3){
		glm::vec3 original = glm::vec3(*(matVecData+i+9), *(matVecData+i+10), 1);
		glm::vec3 transform = matrix * original;

		results[i] = transform.x;
		results[i+1] = transform.y;
		results[i+2] = 1;
	}
}

void matrix3TransformCallback(const MatrixTransformData3D& data){
	glm::mat4 transform = glm::mat4() * Matrix4::ScaleX(data.scaleX) * Matrix4::ScaleY(data.scaleY) * Matrix4::ScaleZ(data.scaleZ) *
		Matrix4::RotationX(data.rotateX) * Matrix4::RotationY(data.rotateY) * Matrix4::RotationZ(data.rotateZ) * glm::mat4(glm::translate(glm::mat4(), glm::vec3(data.translateX, data.translateY, data.translateZ)));
	matrices_3d[data.selectedMatrix] = transform;

	glm::mat4 result = glm::mat4();

	for (int i = 0; i < 10; i++){
		result = result * matrices_3d[i];
	}

	fullTransform = result;

	//glm::mat4 transform = glm::mat4() * Matrix4::ScaleX(data.scaleX) * Matrix4::ScaleY(data.scaleY) * Matrix4::Rotation(data.rotate) * Matrix4::Translation(data.translateX, data.translateY);
	//matrices[data.selectedMatrix] = transform;

	//glm::mat3 result = glm::mat3();

	//for (int i = 0; i < 10; i++){
	//	result = result * matrices[i];
	//}

	//currentTransform = result;
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
	renderUI.set2DMatrixVerticesTransformData(&lines[0].x, numLines, reinterpret_cast<float*>(&matrices), reinterpret_cast<float*>(&currentTransform), matrix2TransformCallback);
	renderUI.set3DMatrixCallback(reinterpret_cast<float*>(&matrices_3d), reinterpret_cast<float*>(&fullTransform), matrix3TransformCallback);

//	void RenderUI::set3DMatrixCallback(
//const float* matrices,
//const float* fulltransform,
//MatrixTransformCallback3D matrixTransformData3DCallback);

	if( ! renderUI.initialize(argc, argv))
		return -1;
	return renderUI.run();
}