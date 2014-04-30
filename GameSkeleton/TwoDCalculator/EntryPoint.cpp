#include "RenderUI.h"
#include "Engine.h"

#include "Vector2.h"

Vector2 left, right, result;
Vector2 original, normal, cwPerp, ccwPerp;
Vector2 vec1, vec2, projectionVec, rejectionVec;
Vector2 aVector, bVector, aMinusB, aVectorLerp, bVectorLerp, lerpResult;

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
		float projScalar = dotProduct(vec1, vec2) / vec2.lengthSquared();
		projectionVec = projScalar * vec2;

		rejectionVec = vec1 - projectionVec;
	}
	else{
		float projScalar = dotProduct(vec2, vec1) / vec1.lengthSquared();
		projectionVec = projScalar * vec1;

		rejectionVec = vec2 - projectionVec;
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

int main(int argc, char* argv[])
{
	Engine::Init();

	RenderUI renderUI;

	renderUI.setBasicVectorEquationData(basicVectorEqCallback, &left.x, &right.x, &result.x);
	renderUI.setPerpendicularData(&original.x, &normal.x, &cwPerp.x, &ccwPerp.x, perpenDataCallback);
	renderUI.setDotProductData(&vec1.x, &vec2.x, &projectionVec.x, &rejectionVec.x, dotProductCallback);
	renderUI.setLerpData(&aVector.x, &bVector.x, &aMinusB.x, &aVectorLerp.x, &bVectorLerp.x, &lerpResult.x, lerpCallback);

	if( ! renderUI.initialize(argc, argv))
		return -1;
	return renderUI.run();
}