#ifndef UTILS_H
#define UTILS_H

#include "Core.h"
#include <stdlib.h>
//#include <math.h>

using Core::Input;
//using namespace Obstacles;

namespace Utils{
	const char TOGGLE_KEY = 'Q';
	const char BG_TOGGLE_KEY = 'E';
	const float E = 2.718f;
	const float PI = 3.14159265f;

	static bool isDisplayed;
	static bool wasToggled;

	static bool isVisible;
	static bool wasBgToggled;

	class Controls{
	public:

		static void ToggleVisibility(){
			isDisplayed = !isDisplayed;
		}
		static void Draw(int x, int y, Core::Graphics& graphics){
			if (isDisplayed){
				graphics.DrawString(x, y, "SPACESHIP CONTROLS");
				graphics.DrawString(x, y + 20, "W = Forward, S = Backward");
				graphics.DrawString(x, y + 40, "A = Rotate Left, D = Rotate Right");
				graphics.DrawString(x, y + 60, "Press left mouse button to fire missiles.");
			}
			else graphics.DrawString(530, 30, "Press 'Q' for controls.");
		}
		static void Update(){
			if (Input::IsPressed(TOGGLE_KEY) && !wasToggled){
				Controls::ToggleVisibility();
			}

			wasToggled = Input::IsPressed(TOGGLE_KEY);
		}
	};

	class BackgroundObjects{
	public:

		static void ToggleVisibility(){
			isVisible = !isVisible;
		}
		static void Draw(Core::Graphics& graphics){
			if (!isVisible){
				graphics.DrawString(530, 55, "Press 'E' to display background objects.");
			}
		}
		static void Update(){
			if (Input::IsPressed(BG_TOGGLE_KEY) && !wasBgToggled){
				BackgroundObjects::ToggleVisibility();
			}

			wasBgToggled = Input::IsPressed(BG_TOGGLE_KEY);
		}
	};

	enum ColorChangeType { FIRE, EXPLOSION, BUBBLE };

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
}

#endif