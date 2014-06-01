#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "ParticleEffect.h"
#include "Vector2.h"

class ExplosionEffect : public ParticleEffect{
private:
	Vector2 position;
	float minVelocity, maxVelocity;
public:
	ExplosionEffect(float dragCoeff, float decceleration, ColorChangeType type,
		Vector2 startPos, float minVel, float maxVel, int numberOfParticles = 1000)
		: ParticleEffect(dragCoeff, decceleration, type, numberOfParticles){
			position = startPos;
			minVelocity = minVel;
			maxVelocity = maxVel;

			int r = 0;
			int g = 0;
			int b = 0;
			float variance = 30.0f;

			if (type == ColorChangeType::RANDOM){
				r = int(Utils::randomInRange(0.0f, 255.0f));
				g = int(Utils::randomInRange(0.0f, 255.0f));
				b = int(Utils::randomInRange(0.0f, 255.0f));
			}

			for (int i = 0; i < numParticles; i++){
				Vector2 velocity = Utils::randomVelocity(minVel, maxVel);
				float LIFETIME = Utils::randomInRange(0.25f, 1.0f);

				particles[i] = Particle(position, velocity, LIFETIME, type);

				if (type == ColorChangeType::RANDOM){
					particles[i]._r = int(Utils::randomInRange((r - variance), (r + variance)));
					particles[i]._g = int(Utils::randomInRange((g - variance), (g + variance)));
					particles[i]._b = int(Utils::randomInRange((b - variance), (b + variance)));
				}
			}
	}

	bool Update(bool isThrusting, float angle, Vector2 pos, float dt){
		isThrusting;
		angle;
		pos;
		
		return Update(dt);
	}

	bool Update(float dt){
		bool isAlive = false;

		for (int i = 0; i < numParticles; i++){
			particles[i].Update(dt);

			if (particles[i].IsAlive()){
				isAlive = true;

				float totalDeccel = deccel + Utils::GaussianFunc(particles[i].GetAge(), 
					(particles[i].GetLifetime() / 2), 0.5f, 0.0f, particles[i].GetAge());

				float oldLength = particles[i].velocity.length();
				float newLength = oldLength - totalDeccel;
				float scalar = newLength / oldLength;

				Vector2 newVelocity = scalar * particles[i].velocity;
				particles[i].velocity = (newVelocity.length() > MIN_VEL) ? newVelocity : Vector2(0.0f, 0.0f);

				particles[i].position = particles[i].position + newVelocity;
			}
		}

		return isAlive;
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
};

#endif