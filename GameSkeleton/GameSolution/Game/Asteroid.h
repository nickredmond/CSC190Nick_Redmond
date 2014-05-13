#include "MoveablleObject.h"
#include "Vector2.h"
#include "Core.h"

class Asteroid : public MoveableObject{
private:
	const static int NUMBER_PATH_LEGS = 4;
	const static int ASTEROID_WIDTH = 15;
	const static int ASTEROID_HEIGHT = 20;
	Vector2 top, right, bottom, left;
	Vector2 path[NUMBER_PATH_LEGS];
	int currentPathLegIndex;
	float beta;

	int GetNextIndex(int index){
		return (index < NUMBER_PATH_LEGS - 1) ? index + 1 : 0;
	}
public:
	Asteroid(Vector2 startingPos){
		path[0] = Vector2(startingPos.x, startingPos.y);
		path[1] = Vector2(startingPos.x + 100, startingPos.y);
		path[2] = Vector2(startingPos.x, startingPos.y + 150);
		path[3] = Vector2(startingPos.x + 100, startingPos.y + 150);

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
	}
	void Draw(Core::Graphics& graphics){
		graphics.DrawLine(top.x, top.y, right.x, right.y);
		graphics.DrawLine(right.x, right.y, bottom.x, bottom.y);
		graphics.DrawLine(bottom.x, bottom.y, left.x, left.y);
		graphics.DrawLine(left.x, left.y, top.x, top.y);
	}
	void Update(float dt){
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
};