#ifndef UPDATE_H
#define UPDATE_H

#include "MoveablleObject.h"

const float GRAVITY = 500.0f;

void UpdateGravity(MoveableObject& obj, float dt){
	if (!obj.isOnGround){
		Vector2 nextVel = obj.GetVelocity() + (Vector2(0.0f, GRAVITY) * dt);
		obj.SetVelocity(nextVel.x, nextVel.y);
	}
}

Vector2 normalizedPerp(Vector2 v1, Vector2 v2){
	Vector2 p = v2 - v1;
	p = p.normalized();
	return p.perpCCW();
}

bool IsCollision(MoveableObject obj, float dt, Vector2 v1, Vector2 v2){
	dt;
	Vector2 n = normalizedPerp(v1, v2);

	Vector2 q1 = obj.GetPosition() - v1;
	float d1 = q1.dotProduct(n);

	d1 -= obj.radius / 2;

	return (d1 < 0 && obj.GetPosition().x > v2.x && obj.GetPosition().x < v1.x);
}

void UpdateColors(ColorChangeType type, Particle* particles, int numParticles){
	for (int i = 0; i < numParticles; i++){
		int redValue = 255;
		int greenValue = 255;
		int blueValue = 255;
		Particle* p = (particles + i);
		float ageRatio = p->GetAge() / p->GetLifetime();

		if (type == ColorChangeType::BUBBLE){
			float blueDecreaseRatio = 0.8f;

			float blueRatio = 1.0f;

			if (ageRatio > blueDecreaseRatio){
				blueRatio = (1.0f - ageRatio) / (1.0f - blueDecreaseRatio);
			}

			redValue = int((1.0f - ageRatio) * 200.0f);
			greenValue = int((1.0f - ageRatio) * 200.0f);
			blueValue = int(blueRatio * 255);
		}
		else if (type == ColorChangeType::FIRE){
			if (ageRatio < 0.33f){
				float blueRatio = 1.0f - (ageRatio / 0.33f);
				blueValue = int(255 * blueRatio);
			}
			else if (ageRatio >= 0.33f && ageRatio < 0.67f){
				float greenRatio = 1.0f - ((ageRatio - 0.33f) / 0.34f);
				greenValue = int(255 * greenRatio);

				blueValue = 0;
			}
			else if (ageRatio < 1.0f){
				float redRatio = 1.0f - ((ageRatio - 0.67f) / 0.33f);
				redValue = int(255 * redRatio);

				blueValue = 0;
				greenValue = 0;
			}
		}
		else if (type == ColorChangeType::RANDOM){
			redValue = p->_r - int(Utils::randomInRange(0.0f, 3.0f));
			greenValue = p->_g - int(Utils::randomInRange(0.0f, 3.0f));
			blueValue = p->_b - int(Utils::randomInRange(0.0f, 3.0f));
		}
		else if (type == ColorChangeType::EXPLOSION){
			// NOT YET IMPLEMENTED
			LOG(Info, "Explosion color change was used in program, but is not yet implemented.");
		}

		p->SetColor(redValue, greenValue, blueValue);
	}
}

#endif