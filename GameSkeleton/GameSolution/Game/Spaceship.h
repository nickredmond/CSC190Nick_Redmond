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
	Turret single, triple, laser;
public:
	int _width, _height;

	Spaceship(int height, int width){
		_width = width;
		_height = height;

		numLines = 6;
		lines = new Vector2[numLines];

		SetPosition(Vector2(0, 0), Vector2(0, 0), 0.0f);

		single = Turret(10, "Single Shot", 10, 100, 10, 5);
		triple = Turret(10, "Triple Shot", 5, 50, 5, 2);
		laser = Turret(10, "Laser", 20, 100, 20, 1.5f);

		gun = single;
	}

	void SetTurret(char* name){
		if (strcmp(gun._name, "Single Shot") == 0){
			single = gun;
		}
		else if (strcmp(gun._name, "Triple Shot") == 0){
			triple = gun;
		}
		else if (strcmp(gun._name, "Laser") == 0){
			laser = gun;
		}

		if (strcmp(name, "Single Shot") == 0){
			gun = single;
		}
		else if (strcmp(name, "Triple Shot") == 0){
			gun = triple;
		}
		else if (strcmp(name, "Laser") == 0){
			gun = laser;
		}
	}
	Turret& GetTurret(){
		return gun;
	}

	void Draw(Core::Graphics& graphics){
		Vector2 translation = Vector2(position.x, position.y);
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