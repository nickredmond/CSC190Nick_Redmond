#ifndef BULLET_H
#define BULLET_H

#include "MoveablleObject.h"

const float BULLET_VELOCITY = 250;

class Bullet : public MoveableObject{
private:
	Matrix3 transform;
	bool _isLaser;

	void SetPosition(Vector2 pos){
		position.x = 0;
		position.y = 0;

		lines[0].x = position.x - 1;
		lines[0].y = position.y - 1;
		lines[1].x = position.x + 1;
		lines[1].y = position.y - 1;
		lines[2].x = position.x + 1;
		lines[2].y = position.y + 1;
		lines[3].x = position.x - 1;
		lines[3].y = position.y + 1; 

		position.x = pos.x;
		position.y = pos.y;
	}
	void SetLaserPosition(Vector2 pos){
		position.x = 0;
		position.y = 0;

		lines[0].x = position.x;
		lines[0].y = position.y;
		lines[1].x = position.x;
		lines[1].y = position.y + 10;
		lines[2].x = position.x;
		lines[2].y = position.y;
		lines[3].x = position.x;
		lines[3].y = position.y + 10; 

		position.x = pos.x;
		position.y = pos.y;
	}
public:
	bool isVisible;

	Bullet(){}

	Bullet(Vector2 vel, Vector2 pos){
		isVisible = false;
		velocity = vel;

		numLines = 4;
		lines = new Vector2[numLines];

		SetPosition(pos);
	}
	Bullet(Vector2 vel, Vector2 pos, bool isLaser){
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
		position = position + (velocity * dt);

		if (!_isLaser){
			angle += dt;
		}
		transform = Matrix3::Rotation(angle - (3.14159365f / 2)).Translate(position.x, position.y);
	}

	void Draw(Core::Graphics& graphics){
		DrawObj(graphics, transform);
	}
};

#endif