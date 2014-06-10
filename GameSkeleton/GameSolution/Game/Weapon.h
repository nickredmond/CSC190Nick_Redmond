#ifndef WEAPON_H
#define WEAPON_H

#include "MoveablleObject.h"
#include "UpdateFunctions.h"
#include "ParticleEffect.h"

const float PROJECTILE_X = 150.0f;
const float WEAPON_RADIUS = 10.0f;

enum ProjectileType { FIREBALL, HAMMER, NONE };
class Weapon : public ParticleEffect {
private:
	Vector2 velocity;
	float minVelocity, maxVelocity;
	ProjectileType pType;
	ColorChangeType _type;
	int screenWidth, screenHeight;

	Particle GetRandomParticle(){
		Vector2 p_velocity = Utils::randomVelocity(minVelocity, maxVelocity);
		float LIFETIME = Utils::randomInRange(0.01f, 0.1f);

		return Particle(position, p_velocity, LIFETIME, _type);
	}
public:
	Weapon(float dragCoeff, float decceleration, ColorChangeType type,
		Vector2 startPos, float minVel, float maxVel, ProjectileType _pType, bool isFacingRight, int numberOfParticles = 1000)
		: ParticleEffect(dragCoeff, decceleration, type, numberOfParticles){
			if(_pType == ProjectileType::FIREBALL){
				float xVel = isFacingRight ? PROJECTILE_X : -PROJECTILE_X;
				velocity = Vector2(xVel, 0.0f);

				position = startPos;
				minVelocity = minVel;
				maxVelocity = maxVel;
				_type = type;

//				float variance = 30.0f;
				pType = _pType;

				for (int i = 0; i < numParticles; i++){
					particles[i] = GetRandomParticle();
				}
			}

			screenWidth = 0;
			screenHeight = 0;
	}
	void SetScreenDimensions(int width, int height){
		screenWidth = width;
		screenHeight = height;
	}

	bool Update(float dt){
		velocity.y = velocity.y + (GRAVITY * dt);
		position = position + (velocity * dt);

		for (int i = 0; i < numParticles; i++){
			particles[i].Update(dt);

			if (particles[i].IsAlive()){
				float totalDeccel = deccel + Utils::GaussianFunc(particles[i].GetAge(), 
					(particles[i].GetLifetime() / 2), 0.5f, 0.0f, particles[i].GetAge());

				float oldLength = particles[i].velocity.length();
				float newLength = oldLength - totalDeccel;
				float scalar = newLength / oldLength;

				Vector2 newVelocity = scalar * particles[i].velocity;
				particles[i].velocity = (newVelocity.length() > MIN_VEL) ? newVelocity : Vector2(0.0f, 0.0f);

				particles[i].position = particles[i].position + newVelocity;
			}
			else{
				particles[i] = GetRandomParticle();
			}
		}

		return (screenWidth == 0 && screenHeight == 0) ||
			(position.x > 0 && position.x < screenWidth && position.y > 0 && position.y < screenHeight);
	}

	bool Update(bool isThrusting, float angle, Vector2 pos, float dt) {
		isThrusting;
		angle;
		pos;
		return Update(dt);
	}
	void Draw(Core::Graphics& graphics){
		for (int i = 0; i < numParticles; i++){
			if (particles[i].IsAlive()){
				graphics.SetColor(particles[i].GetColor());

				float x = particles[i].position.x;
				float y = particles[i].position.y;
				graphics.DrawLine(x, y, (x + 3), y);
			}
		}
	}

	void Bounce(){
		velocity.y = -100.0f;
	}
	Vector2 GetVelocity(){
		return velocity;
	}
};

#endif