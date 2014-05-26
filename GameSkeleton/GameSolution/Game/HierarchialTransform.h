#ifndef HIER_H
#define HIER_H

#include "Vector2.h"
#include "MoveablleObject.h"

const float ROT_VEL = 0.45f;
const float COLOR_VEL = 100.0f;

class Color{
public:
	float red, blue, green;

	Color(float r = 255.0f, float g = 255.0f, float b = 255.0f){
		red = r;
		green = g;
		blue = b;
	}
};

class HierarchialTransform : public MoveableObject{
private:
	float _scale, _translation;
	int numShapes;
	Color currentColor;
	bool isTurningRed, isTurningYellow, isTurningGreen, isTurningBlue, isTurningPurple;

	void SetPosition(Vector2 pos){
		position.x = 0;
		position.y = 0;

		lines[0].x = position.x - 100;
		lines[0].y = position.y - 100;
		lines[1].x = position.x + 100;
		lines[1].y = position.y - 100;
		lines[2].x = position.x + 100;
		lines[2].y = position.y + 100;
		lines[3].x = position.x - 100;
		lines[3].y = position.y + 100; 

		position.x = pos.x;
		position.y = pos.y;
	}

	void UpdateColor(float dt){
		if (isTurningRed){
			currentColor.blue = currentColor.blue - (COLOR_VEL * dt);

			if (currentColor.blue <= 0){
				isTurningRed = false;
				isTurningYellow = true;
			}
		}
		else if (isTurningYellow){
			currentColor.green = currentColor.green + (COLOR_VEL * dt);

			if (currentColor.green >= 255){
				isTurningYellow = false;
				isTurningGreen = true;
			}
		}
		else if (isTurningGreen){
			currentColor.red = currentColor.red - (COLOR_VEL * dt);

			if (currentColor.red <= 0){
				isTurningGreen = false;
				isTurningBlue = true;
			}
		}
		else if (isTurningBlue){
			currentColor.green = currentColor.green - (COLOR_VEL * dt);
			currentColor.blue = currentColor.blue + (COLOR_VEL * dt);

			if (currentColor.blue >= 255){
				isTurningBlue = false;
				isTurningPurple = true;
			}
		}
		else if (isTurningPurple){
			currentColor.red = currentColor.red + (COLOR_VEL * dt);

			if (currentColor.red >= 255){
				isTurningPurple = false;
				isTurningRed = true;
			}
		}

		if (currentColor.red >= 256){
			currentColor.red = 255;
		}
		if (currentColor.green >= 256){
			currentColor.green = 255;
		}
		if (currentColor.blue >= 256){
			currentColor.blue = 255;
		}

		if (currentColor.red <= -1){
			currentColor.red = 0;
		}
		if (currentColor.green <= -1){
			currentColor.green = 0;
		}
		if (currentColor.blue <= -1){
			currentColor.blue = 0;
		}
	}

	void DrawShape(Core::Graphics& graphics, int shapeNumber, Matrix3 currentTransform, float currentScale, 
		float currentTranslation){
			DrawObj(graphics, currentTransform.Translate(position.x, position.y) * Matrix3::Rotation(angle));

		float newScale = _scale * currentScale;
		float newTranslation = currentTranslation + _translation;
		Matrix3 newTransform = (currentTransform * Matrix3::Scale(newScale).Translate(newTranslation, 0.0f) * Matrix3::Rotation(0));

		if (shapeNumber < numShapes){
			DrawShape(graphics, (shapeNumber + 1), newTransform, newScale, newTranslation);
		}
	}
public:
	HierarchialTransform(){}

	HierarchialTransform(float scale, float translation, int numberOfShapes, Vector2 pos){
		_scale = scale;
		_translation = translation;
		numShapes = numberOfShapes;

		numLines = 4;
		lines = new Vector2[numLines];

		SetPosition(pos);
		isTurningYellow = true;
		currentColor = Color(255, 0, 0);
		angle = 0.0f;
	}

	void Update(float dt){
		angle += ROT_VEL * dt;
		UpdateColor(dt);
	}

	void Draw(Core::Graphics& graphics){
		Core::RGB objColor = RGB(int(currentColor.red), int(currentColor.green), int(currentColor.blue));
		graphics.SetColor(objColor);

		Matrix3 transform = Matrix3::Rotation(angle);
		DrawShape(graphics, 1, transform, 1.0f, 0);
	}
};

#endif