#ifndef ENEMY_H
#define ENEMY_H

#include "Core.h"
#include "Turret.h"

class Enemy : public MoveableObject{
public:
	virtual void Update(Vector2 shipPos, float dt) = 0;
	virtual void Draw(Core::Graphics& graphics) = 0;
};

#endif