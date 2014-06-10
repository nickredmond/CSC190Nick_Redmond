#ifndef BUBBLE_H
#define BUBBLE_H

#include "ParticleEffect.h"
#include "Vector2.h"

const float MIN_LIFETIME = 0.0f;
const float MAX_LIFETIME = 3.0f;

const float MIN_R_DEFAULT = 1.5f;
const float MAX_R_DEFAULT = 15.0f;

class BubbleEffect : public ParticleEffect{
private:
	float minVelocity, maxVelocity, _angle, _variance, minRadius, maxRadius;
	bool _isThrusting;
	// (2 PI) / (2 PI r)

public:
	BubbleEffect(float dragCoeff, float decceleration, ColorChangeType type,
		Vector2 startPos, float variance, float angle, float minVel, float maxVel, float minR = MIN_R_DEFAULT, float maxR = MAX_R_DEFAULT, int numberOfParticles = 1000)
		: ParticleEffect(dragCoeff, decceleration, type, numberOfParticles){
			position = startPos;
			minVelocity = minVel;
			maxVelocity = maxVel;
			_angle = angle;
			_variance = variance;

			minRadius = minR;
			maxRadius = maxR;

			//for (int i = 0; i < numParticles; i++){
			//	Vector2 velocity = Utils::randomInRange(minVel, maxVel) * Utils::randomUnitVector(angle, variance);
			//	float LIFETIME = Utils::randomInRange(MIN_LIFETIME, MAX_LIFETIME);

			//	particles[i] = Particle(position, velocity, LIFETIME, type);
			//	particles[i].radius = Utils::randomInRange(2.0f, 20.0f);
			//}
	}

	void SetAngle(float angle){
		_angle = angle;
	}
	void SetPosition(Vector2 pos){
		position.x = pos.x;
		position.y = pos.y;
	}

	void Draw(Core::Graphics& graphics){
		for (int i = 0; i < numParticles; i++){
			if (particles[i].IsAlive()){
				graphics.SetColor(particles[i].GetColor());
				Utils::Draw_MidpointCircle(graphics, particles[i].radius, particles[i].position);
			}
		}
	}

	bool Update(bool isThrusting, float angle, Vector2 pos, float dt){
		_isThrusting = isThrusting;
		_angle = angle;
		SetPosition(pos);

		return Update(dt);
	}
	bool Update(float dt){
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
			else if (_isThrusting){
				Vector2 velocity = Utils::randomInRange(minVelocity, maxVelocity) * Utils::randomUnitVector(_angle, _variance);
				float LIFETIME = Utils::randomInRange(MIN_LIFETIME, MAX_LIFETIME);

				particles[i] = Particle(position, velocity, LIFETIME, colorChange);
				particles[i].radius = Utils::randomInRange(minRadius, maxRadius);
			}
		}

		return true;
	}
};

#endif