#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "Engine.h"
#include "Vector2.h"
#include "MoveablleObject.h"

const float ACCELERATION = 8.0f;

class Spaceship : public MoveableObject{
private: 
	struct Vector2 right, bottomRight, bottom, bottomLeft, left;
public:
	int _width, _height;

	Spaceship(int height, int width){
		_width = width;
		_height = height;

		SetPosition(Vector2(0, 0));
	}
	void Draw(Core::Graphics& graphics){
		graphics.DrawLine(position.x, position.y, right.x, right.y);
		graphics.DrawLine(right.x, right.y, bottomRight.x, bottomRight.y);
		graphics.DrawLine(bottomRight.x, bottomRight.y, bottom.x, bottom.y);
		graphics.DrawLine(bottom.x, bottom.y, bottomLeft.x, bottomLeft.y);
		graphics.DrawLine(bottomLeft.x, bottomLeft.y, left.x, left.y);
		graphics.DrawLine(left.x, left.y, position.x, position.y);
	}

	void SetPosition(Vector2 pos){
		position.x = pos.x;
		position.y = pos.y;

		right.x = position.x + (_width / 2);
		right.y = position.y + (0.33f * _height);
		bottomRight.x = position.x + (_width / 4);
		bottomRight.y = position.y + (0.67f * _height);
		bottom.x = position.x;
		bottom.y = position.y + _height;
		left.x = position.x - (_width / 2);
		left.y = position.y + (0.33f * _height);
		bottomLeft.x = position.x - (_width / 4);
		bottomLeft.y = position.y + (0.67f * _height);
	}

	void UpdatePosition(float xVelDelta, float yVelDelta, float dt){
		velocity.x += xVelDelta;
		velocity.y += yVelDelta;

		Vector2 newPos = position + (velocity * dt);
		SetPosition(newPos);
	}
};

#endif