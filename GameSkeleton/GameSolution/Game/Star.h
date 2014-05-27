#ifndef STAR_H
#define STAR_H

#include "Vector2.h"
#include "Core.h"
#include "Utils.h"

class Star{
private:
	Vector2 position;
	float brightness, maxBrightness, age, lifetime;
public:
	Star(){
		age = 0.0f;
		lifetime = 0.0f;
		brightness = 0.0f;
		maxBrightness = 0.0f;
	}

	Star(Vector2 pos, float _maxBrightness, float _lifetime){
		position = pos;
		maxBrightness = _maxBrightness;
		lifetime = _lifetime;

		age = 0.0f;
	}
	
	bool IsAlive(){
		return (age < lifetime);
	}
	
	void Update(float dt){
		age += dt;
		brightness = Utils::GaussianFunc(1.0f, (lifetime / 2.0f), 0.5f, 0.0f, age);
	}
	void Draw(Core::Graphics& graphics){
		int normalizedVal = int(255.0f * brightness);
		graphics.SetColor(RGB(normalizedVal, normalizedVal, normalizedVal));

		graphics.DrawLine(position.x - 1, position.y, position.x + 1, position.y);
		graphics.DrawLine(position.x, position.y - 1, position.x, position.y + 1);
	}
};

#endif