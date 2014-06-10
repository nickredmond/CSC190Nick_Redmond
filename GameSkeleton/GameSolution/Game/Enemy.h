#ifndef ENEMY_H
#define ENEMY_H

#include "MoveablleObject.h"

class Enemy : public MoveableObject{
private:
	bool isAlive;
public:
	Enemy(){
		isAlive = true;
	}

	void SwitchDirection(){
		velocity.x = -velocity.x;
	}

	//void AvoidWallEdges(Wall* walls, int numWalls){
	//	for (int i = 0; i < numWalls; i++){
	//		Wall nextWall = *(walls+i);

	//		if ((nextWall.v1.x > position.x && nextWall.v2.x < position.x) ||
	//			(nextWall.v2.x > position.x && nextWall.v1.x < position.x)){
	//			
	//		}
	//	}
	//}

	bool Update(float dt){
		if (isAlive){
			position = position + (velocity * dt);
		}

		return isAlive;
	}
	void Draw(Core::Graphics& graphics){
		if (isAlive){
			_transform = Matrix3::Translation(position.x, position.y);
			DrawObj(graphics);
		}
	}
	void Hurt(){
		isAlive = false;
	}
	bool IsAlive(){
		return isAlive;
	}
};

#endif