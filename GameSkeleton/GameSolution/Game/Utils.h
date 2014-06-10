#ifndef UTILS_H
#define UTILS_H

#include "Core.h"
#include <stdlib.h>
//#include <math.h>

using Core::Input;
//using namespace Obstacles;

const float E = 2.718f;
const float PI = 3.14159265f;

enum ColorChangeType { FIRE, EXPLOSION, BUBBLE, RANDOM };

namespace Utils{
	float randomFloat(){
		return float(rand()) / RAND_MAX;
	}

	float randomInRange(float min, float max){
		return randomFloat() * (max - min + 1) + min;
	}

	float GaussianFunc(float curveHeight, float peakPos, float steepness,
		float asymptote, float x){
			float powerNumerator = pow((x - peakPos), 2);
			float powerDenominator = 2 * (steepness * steepness);
			float power = -(powerNumerator / powerDenominator);

			return (curveHeight * pow(E, power)) + asymptote;
	}

	Vector2 randomUnitVector(){
		float angle = (2 * PI) * randomFloat();
		Vector2 result = Vector2(cos(angle), sin(angle));
		return result;
	}

	Vector2 randomUnitVector(float angle, float variance){
		float variedAngle = (randomInRange((angle - variance), (angle + variance))) - 0.5f;
		Vector2 result = Vector2(sin(variedAngle), cos(variedAngle));
		return result;
	}

	Vector2 randomVelocity(float min, float max){
		float angle = (2 * PI) * randomFloat();
		Vector2 result = Vector2(cos(angle), sin(angle)) * randomInRange(min, max);
		return result;
	}

	float Perimeter(float width, float height){
		return (2 * width) + (2 * height);
	}

	void Draw_MidpointCircle(Core::Graphics& graphics, float radius, Vector2 position){
		// float circumference = 2 * Utils::PI * radius;
		float radiansPerInc = 1.0f / radius; //(2 * Utils::PI) / circumference;
		float angle = 0.0f;

		do{
			Vector2 translation = Utils::randomUnitVector() * radius;
			Vector2 pixelPos = position + translation;
			graphics.DrawLine(pixelPos.x - 1, pixelPos.y, (pixelPos.x + 1), pixelPos.y);
			graphics.DrawLine(pixelPos.x, pixelPos.y - 1, pixelPos.x , pixelPos.y + 1);

			angle += radiansPerInc;
		}while(angle < (2.0f * PI));
	}
}

#endif