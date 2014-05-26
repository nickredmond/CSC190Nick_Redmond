#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector2.h"
#include "Core.h"
#include "Utils.h"

using Core::RGB;
using Utils::ColorChangeType;

class Particle{
private:
	float _lifetime, age;
	RGB color;
	ColorChangeType colorChange;
public:
	Vector2 position, velocity;
	float radius, angle;

	int _r, _g, _b;

	Particle(){
		age = 0.0f;
		_lifetime = 0.0f;
	}

	Particle(Vector2 pos, Vector2 vel, float lifetime, ColorChangeType type){
		position = pos;
		velocity = vel;
		_lifetime = lifetime;
		colorChange = type;
		
		age = 0.0f;
	}

	void Update(float dt){
		age += dt;
	}

	float GetLifetime(){
		return _lifetime;
	}
	float GetAge(){
		return age;
	}
	bool IsAlive(){
		return (age < _lifetime);
	}
	RGB GetColor(){
		//return color;
		return RGB(_r, _g, _b);
	}
	void SetColor(int r, int g, int b){
		color = RGB(r, g, b);
		_r = r;
		_g = g;
		_b = b;
	}
};

#endif