#ifndef ASTEROID_H
#define ASTEROID_H

#include "MoveablleObject.h"
#include "Vector2.h"
#include "Core.h"

const float DEFAULT_ACCEL = 4.0f;

class Asteroid : public MoveableObject{
private:
	const static int NUMBER_PATH_LEGS = 4;
	const static int ASTEROID_WIDTH = 15;
	const static int ASTEROID_HEIGHT = 20;
	Vector2 path[NUMBER_PATH_LEGS];
	Vector2 top, right, bottom, left;
	int currentPathLegIndex;
	float beta;
	bool isLerping;

	int GetNextIndex(int index){
		return (index < NUMBER_PATH_LEGS - 1) ? index + 1 : 0;
	}
public:
	Asteroid(){numLines = 8;}
	Asteroid(Vector2 startingPos, bool _isLerping = true, float _maxHealth = 100){
		path[0] = Vector2(startingPos.x, startingPos.y);
		path[1] = Vector2(startingPos.x + 100, startingPos.y);
		path[2] = Vector2(startingPos.x, startingPos.y + 150);
		path[3] = Vector2(startingPos.x + 100, startingPos.y + 150);

		isLerping = _isLerping;
		maxHealth = _maxHealth;
		currentHealth = maxHealth;

		numLines = 8;
		lines = new Vector2[numLines];

		position.x = path[0].x;
		position.y = path[0].y;
	}

	void SetPosition(Vector2 pos){
		position.x = pos.x;
		position.y = pos.y;

		top.x = pos.x;
		top.y = pos.y - (ASTEROID_HEIGHT / 2);
		right.x = pos.x + (ASTEROID_WIDTH / 2);
		right.y = pos.y;
		bottom.x = pos.x;
		bottom.y = pos.y + (ASTEROID_HEIGHT / 2);
		left.x = pos.x - (ASTEROID_WIDTH / 2);
		left.y = pos.y;

		lines[0].x = top.x;
		lines[0].y = top.y;
		lines[1].x = right.x;
		lines[1].y = right.y;
		lines[2].x = right.x;
		lines[2].y = right.y;
		lines[3].x = bottom.x;
		lines[3].y = bottom.y;
		lines[4].x = bottom.x;
		lines[4].y = bottom.y;
		lines[5].x = left.x;
		lines[5].y = left.y;
		lines[6].x = left.x;
		lines[6].y = left.y;
		lines[7].x = top.x;
		lines[7].y = top.y;
	}
	void Draw(Core::Graphics& graphics){
		graphics.DrawLine(top.x, top.y, right.x, right.y);
		graphics.DrawLine(right.x, right.y, bottom.x, bottom.y);
		graphics.DrawLine(bottom.x, bottom.y, left.x, left.y);
		graphics.DrawLine(left.x, left.y, top.x, top.y);
	}
	void Update(float dt, Vector2 shipPos){
		if (isLerping){
			float distanceTraveled = velocity.length() * dt;
			float legDistance = distance(path[currentPathLegIndex], path[GetNextIndex(currentPathLegIndex)]);
			float deltaBeta = distanceTraveled / legDistance;

			if (beta + deltaBeta >= 1){
				beta = deltaBeta - (1 - beta);
				currentPathLegIndex = GetNextIndex(currentPathLegIndex);
			}
			else beta += deltaBeta;

			Vector2 start = path[currentPathLegIndex];
			Vector2 end = path[GetNextIndex(currentPathLegIndex)];

			Vector2 currentPos = LERP(start, end, beta);
			SetPosition(currentPos);
		}
		else{
			Vector2 shipMinusPos = shipPos - position;
			Vector2 newVel = shipMinusPos.normalized() * DEFAULT_ACCEL;
			velocity.x = newVel.x;
			velocity.y = newVel.y;

			Vector2 pos = position + velocity;
			SetPosition(pos);
		}
	}

	bool IsAlive(){
		return (currentHealth > 0.0f);
	}
};

#endif