#ifndef PLAYER_H
#define PLAYER_H

#include "MoveablleObject.h"
#include "Debug.h"
#include "Weapon.h"
using Core::Input;

const float JUMP_VELOCITY = -400.0f;
const float MAX_VELOCITY = 350.0f;
const float PLAYER_ACCEL = 2.0f;
const float DRAG = 0.065f;
const float MIN_VELOCITY = 0.01f;

class Player : public MoveableObject{
private:
	int numberLives;
public:
	bool isAlive, isBig;
	bool isFacingRight;
	bool isScrolling;
	ProjectileType _type;

	Player(){
		_type = ProjectileType::NONE; // ProjectileType::NONE;
		numLines = 8;
		lines = new Vector2[numLines];
		radius = 15.0f;

		isOnGround = false;
		isAlive = true;
		isBig = false;

		position.x = 0;
		position.y = 0;

		lines[0].x = position.x - 10;
		lines[0].y = position.x - 20;
		lines[1].x = position.x + 10;
		lines[1].y = position.y - 20;
		lines[2].x = position.x + 10;
		lines[2].y = position.y - 20;
		lines[3].x = position.x + 10;
		lines[3].y = position.y + 20;
		lines[4].x = position.x + 10;
		lines[4].y = position.y + 20;
		lines[5].x = position.x - 10;
		lines[5].y = position.x + 20;
		lines[6].x = position.x - 10;
		lines[6].y = position.x + 20;
		lines[7].x = position.x - 10;
		lines[7].y = position.x - 20;

		position.x = 500;
		position.y = 200;
	}

	bool Update(float dt){
		if (Input::IsPressed('A')){
			isFacingRight = false;
			if (velocity.x > -MAX_VELOCITY){
				velocity.x -= PLAYER_ACCEL;
			}
		}
		else if (velocity.x < 0){
			velocity.x += DRAG;
		}

		if (Input::IsPressed('D')){
			isFacingRight = true;
			if (velocity.x < MAX_VELOCITY){
				velocity.x += PLAYER_ACCEL;
			}
		}
		else if (velocity.x > 0){
			velocity.x -= DRAG;
		}

		if (Input::IsPressed('W')){
			if(isOnGround){
				velocity.y = JUMP_VELOCITY;
				isOnGround = false;
			}
		}
		if (Input::IsPressed('S')){
			// if he's big, make him crouch
		}

		if (!isScrolling){
			position.x = position.x + (velocity.x * dt);
		}
		position.y = position.y + (velocity.y * dt);

		_transform = Matrix3::Translation(position.x, position.y);
		dt;

		if (velocity.x < MIN_VELOCITY && velocity.x > -MIN_VELOCITY){
			velocity.x = 0.0f;
		}

		return true;
	}
	void Draw(Core::Graphics& graphics){
		if (_type == ProjectileType::FIREBALL){
			Core::RGB fColor = RGB(255, 255, 0);
			graphics.SetColor(fColor);
		}
		DrawObj(graphics);

		Core::RGB color = RGB(255, 255, 255);
		graphics.SetColor(color);
	}

	void Hurt(){
		if (_type != ProjectileType::NONE){
			_type = ProjectileType::NONE;
		}
		else if (isBig){
			isBig = false;
		}
		else{
			isAlive = false;
			isOnGround = false;
		}
	}
};

#endif