#ifndef KOOPA_H
#define KOOPA_H

#include "Enemy.h"
#include "DebugMemory.h"

class Koopa : public Enemy{
public:
	Koopa(){
		numLines = 8;
		lines = new Vector2[numLines];
		position = Vector2();

		lines[0].x = position.x - 20;
		lines[0].y = position.x - 20;
		lines[1].x = position.x + 20;
		lines[1].y = position.y - 20;
		lines[2].x = position.x + 20;
		lines[2].y = position.y - 20;
		lines[3].x = position.x + 20;
		lines[3].y = position.y + 20;
		lines[4].x = position.x + 20;
		lines[4].y = position.y + 20;
		lines[5].x = position.x - 20;
		lines[5].y = position.x + 20;
		lines[6].x = position.x - 20;
		lines[6].y = position.x + 20;
		lines[7].x = position.x - 20;
		lines[7].y = position.x - 20;

		velocity = Vector2(-50.0f, 0.0f);
		radius = 20.0f;
	}
};

#endif