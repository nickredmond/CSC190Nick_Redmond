#ifndef PARTICLE_EFFECT_H
#define PARTICLE_EFFECT_H

#include "Particle.h"
#include "Core.h"
#include "Utils.h"
#include "DebugMemory.h"

const float MIN_VEL = 0.001f;

class ParticleEffect{
protected:
	Vector2 position;
	Particle* particles;
	ColorChangeType colorChange;
	float drag, deccel;
	int numParticles;
public:
	ParticleEffect(float dragCoeff, float decceleration, ColorChangeType type,
		int numberOfParticles = 1000){
		drag = dragCoeff;
		deccel = decceleration;
		colorChange = type;
		numParticles = numberOfParticles;

		particles = new Particle[numParticles];
	}
	~ParticleEffect(){
		int x = 0;
		x;
	}
	void Destroy(){
		delete [] particles;
	}

	Particle* GetParticles(){
		return particles;
	}
	ColorChangeType GetColorChangeType(){
		return colorChange;
	}
	int GetNrParticles(){
		return numParticles;
	}
	Vector2 GetPosition(){
		return position;
	}
	void SetPosition(Vector2 pos){
		position.x = pos.x;
		position.y = pos.y;
	}

	virtual bool Update(float dt) = 0;
	virtual bool Update(bool isThrusting, float angle, Vector2 pos, float dt) = 0;
	virtual void Draw(Core::Graphics& graphics) = 0;
};

#endif