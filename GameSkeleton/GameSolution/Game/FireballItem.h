#ifndef FBALL_ITEM_H
#define FBALL_ITEM_H

#include "Item.h"

class FireballItem : public Item{
public:
	FireballItem(Vector2 pos){
		initialized = true;
		radius = 10.0f;

		position.x = 0.0f;
		position.y = 0.0f;

		numLines = 14;
		lines = new Vector2[numLines];

		lines[0].x = position.x - 10;
		lines[0].y = position.y - 8;
		lines[1].x = position.x + 10;
		lines[1].y = position.y - 8;
		lines[2].x = position.x + 10;
		lines[2].y = position.y - 8;
		lines[3].x = position.x + 10;
		lines[3].y = position.y;
		lines[4].x = position.x + 10;
		lines[4].y = position.y;
		lines[5].x = position.x - 10;
		lines[5].y = position.y;
		lines[6].x = position.x - 10;
		lines[6].y = position.y;
		lines[7].x = position.x - 10;
		lines[7].y = position.y - 8;
		lines[8].x = position.x;
		lines[8].y = position.y;
		lines[9].x = position.x,
		lines[9].y = position.y + 5;
		lines[10].x = position.x,
		lines[10].y = position.y + 5;
		lines[11].x = position.x + 5;
		lines[11].y = position.y + 3;
		lines[12].x = position.x - 5;
		lines[12].y = position.y + 3;
		lines[13].x = position.x,
		lines[13].y = position.y + 5;

		position.x = pos.x;
		position.y = pos.y;
	}

	void Use(Player& plyr){
		plyr._type = ProjectileType::FIREBALL;
		initialized = false;
	}
	bool Update(float dt){
		dt;
		_transform = Matrix3::Translation(position);
		return true;
	}
	void Draw(Core::Graphics& graphics){
		DrawObj(graphics);
	}
};

#endif