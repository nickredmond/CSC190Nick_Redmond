#ifndef MOVEABLE_H
#define MOVEABLE_H

class MoveableObject{
protected:
	Vector2 position, velocity;
public:
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
};

#endif