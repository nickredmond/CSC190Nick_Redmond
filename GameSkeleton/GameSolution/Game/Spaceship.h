#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "Engine.h"
#include "Vector2.h"
#include "MoveablleObject.h"
#include "Turret.h"
#include "Debug.h"

class Spaceship : public MoveableObject{
private: 
	struct Vector2 right, bottomRight, bottom, bottomLeft, left;
	Turret gun;
public:
	int _width, _height;

	Spaceship(int height, int width){
		_width = width;
		_height = height;

		numLines = 6;
		lines = new Vector2[numLines];

		SetPosition(Vector2(0, 0), Vector2(0, 0), 0.0f);

		gun = Turret(10, "Single Shot", 10, 100, 10, 1000);
	}

	void SetTurret(char* name){
		gun = Turret(10, name, 10, 100, 10, 1000);
	}
	Turret GetTurret(){
		return gun;
	}

	//void Draw(Core::Graphics& graphics){
	//	graphics.DrawLine(position.x, position.y, right.x, right.y);
	//	graphics.DrawLine(right.x, right.y, bottomRight.x, bottomRight.y);
	//	graphics.DrawLine(bottomRight.x, bottomRight.y, bottom.x, bottom.y);
	//	graphics.DrawLine(bottom.x, bottom.y, bottomLeft.x, bottomLeft.y);
	//	graphics.DrawLine(bottomLeft.x, bottomLeft.y, left.x, left.y);
	//	graphics.DrawLine(left.x, left.y, position.x, position.y);
	//}

	void Draw(Core::Graphics& graphics){
		Vector2 translation = Vector2(position.x, position.y); //(velocity * prev_dt);
		Matrix3 transform = Matrix3::Translation(translation) * Matrix3::Rotation(angle);

		Debug::DrawMatrix(300, 550, graphics, transform);

		DrawObj(graphics, transform);
		gun.Draw(graphics);
	}

	void SetPosition(Vector2 pos, Vector2 mousePos, float dt){
		position.x = 0;
		position.y = 0;

		lines[0].x = position.x;
		lines[0].y = position.y - (float(_height) / 2);
		lines[1].x = position.x + (float(_width) / 2);
		lines[1].y = position.y;
		lines[2].x = position.x + (float(_width) / 4);
		lines[2].y = position.y + (0.67f * _height);
		lines[3].x = position.x;
		lines[3].y = position.y + (0.33f * _height); 
		lines[4].x = position.x - (float(_width) / 4);
		lines[4].y = position.y + (0.67f * _height);
		lines[5].x = position.x - (float(_width) / 2);
		lines[5].y = position.y;

		position.x = pos.x;
		position.y = pos.y;

		gun.Update(position, mousePos, dt);
	}
};

#endif