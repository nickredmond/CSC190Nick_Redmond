#ifndef MOVEABLE_H
#define MOVEABLE_H

#include "core.h"
#include "Matrix3.h"

const float ACCELERATION = 8.0f;

class MoveableObject{
protected:
	Vector2 position, velocity;
	float currentHealth, maxHealth;
public:
	Vector2* lines;
	int numLines;
	float angle, prev_dt, omega;
	float collisionRadius;

	MoveableObject(){
		omega = 0.05f;
	}
	//~MoveableObject(){
	//	delete [] lines;
	//}

	void SetVelocity(float xVel, float yVel){
		velocity.x = xVel;
		velocity.y = yVel;
	}

	Vector2 GetPosition(){
		return position;
	}
	Vector2 GetVelocity(){
		return velocity;
	}

	void DrawObj(Core::Graphics& graphics, Matrix3 transform){
		for (int i = 0; i < numLines - 1; i+=2){
			Vector2 p1 = transform * lines[i];
			Vector2 p2 = transform * lines[i+1];
			graphics.DrawLine(p1.x, p1.y, p2.x, p2.y);
		}

		//Vector2 p1 = transform * lines[numLines - 1];
		//Vector2 p2 = transform * lines[0];
		//graphics.DrawLine(p1.x, p1.y, p2.x, p2.y);
	}

	void Kill(){
		currentHealth = 0;
	}

	bool IsAlive(){
		return (currentHealth > 0.0f);
	}
};

#endif