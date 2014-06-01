#ifndef BULLET_H
#define BULLET_H

#include "MoveablleObject.h"

const float BULLET_VELOCITY = 250;
Core::RGB bulletColor = RGB(160, 160, 255);

class Bullet : public MoveableObject{
private:
	Matrix3 transform;
	bool _isLaser;

	void SetPosition(Vector2 pos){
		position.x = 0;
		position.y = 0;

		lines[0].x = position.x - 2;
		lines[0].y = position.y - 2;
		lines[1].x = position.x + 2;
		lines[1].y = position.y - 2;
		lines[2].x = position.x + 2;
		lines[2].y = position.y - 2;
		lines[3].x = position.x + 2;
		lines[3].y = position.y + 2;
		lines[4].x = position.x + 2;
		lines[4].y = position.y + 2;
		lines[5].x = position.x - 2;
		lines[5].y = position.y + 2; 
		lines[6].x = position.x - 2;
		lines[6].y = position.y + 2; 
		lines[7].x = position.x - 2;
		lines[7].y = position.y - 2;

		position.x = pos.x;
		position.y = pos.y;
	}
	void SetLaserPosition(Vector2 pos){
		position.x = 0;
		position.y = 0;

		lines[0].x = position.x;
		lines[0].y = position.y;
		lines[1].x = position.x;
		lines[1].y = position.y + 12;
		lines[2].x = position.x;
		lines[2].y = position.y;
		lines[3].x = position.x;
		lines[3].y = position.y + 12; 

		position.x = pos.x;
		position.y = pos.y;
	}
public:
	bool isVisible, isNull;

	Bullet(){
		isNull = true;
	}

	Bullet(Vector2 vel, Vector2 pos){
		isVisible = false;
		velocity = vel;

		numLines = 8;
		lines = new Vector2[numLines];

		SetPosition(pos);
	}
	Bullet(Vector2 vel, Vector2 pos, bool isLaser){
		isNull = false;

		isVisible = false;
		velocity = vel;

		numLines = 4;
		lines = new Vector2[numLines];

		if (isLaser){
			SetLaserPosition(pos);
		}
		else SetPosition(pos);

		_isLaser = isLaser;
	}

	Matrix3 GetTransform(){
		return transform;
	}

	void Update(float dt){
		isNull = false;
		position = position + (velocity * dt);

		if (!_isLaser){
			angle += dt;
		}
		transform = Matrix3::Rotation(angle - (3.14159365f / 2)).Translate(position.x, position.y);
	}

	void Draw(Core::Graphics& graphics){
		if(!isNull){
			graphics.SetColor(bulletColor);
			DrawObj(graphics, transform);
		}
	}

	bool IsAlive(){return true;}
};

#endif