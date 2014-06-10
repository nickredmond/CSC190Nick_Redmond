#ifndef MOVEABLE_H
#define MOVEABLE_H

#include "Matrix3.h"
#include "GameObject.h"

const float ACCELERATION = 8.0f;

class MoveableObject : public GameObject{
protected:
	Vector2 velocity;
	Matrix3 _transform;
public:
	bool isOnGround;
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

	Vector2 GetVelocity(){
		return velocity;
	}

	void DrawObj(Core::Graphics& graphics){
		DrawObj(graphics, _transform);
	}
	void DrawObj(Core::Graphics& graphics, Matrix3 transform){
		for (int i = 0; i < numLines - 1; i+=2){
			Vector2 p1 = transform * lines[i];
			Vector2 p2 = transform * lines[i+1];
			graphics.DrawLine(p1.x, p1.y, p2.x, p2.y);
		}
	}

	bool Update(float dt){dt;return true;}
	void Draw(Core::Graphics& graphics){graphics;}
};



#endif