#ifndef COIN_H
#define COIN_H

#include "MoveablleObject.h"
#include "Utils.h"

const float COIN_DECCEL = 0.05f;
const float COIN_VEL = -5.0f;

class Coin : public MoveableObject{
private:
	float lifetime, age;
public:
	Coin(){}

	Coin(Vector2 pos){
		position.x = pos.x;
		position.y = pos.y;

		radius = 8.0f;

		lifetime = 1.0f;
		age = 0.0f;
		velocity = Vector2(0.0f, COIN_VEL);
	}
	bool IsAlive(){
		return age < lifetime;
	}
	bool Update(float dt){
		age += dt;
		velocity.y = velocity.y + COIN_DECCEL;
		position = position + velocity;

		return true;
	}
	void Draw(Core::Graphics& graphics){
		int redValue = int(255.0f - (255.0f * (age / lifetime)));
		int greenValue = int(255.0f - (255.0f * (age / lifetime)));
		Core::RGB color = RGB(redValue, greenValue, 0);
		graphics.SetColor(color);

		Utils::Draw_MidpointCircle(graphics, radius, position);
	}
};

#endif