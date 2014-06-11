#ifndef BLOCK_H
#define BLOCK_H

#include "NullItem.h"
#include "DebugMemory.h"

const float BLOCK_ACCEL = 485.0f;

class Block : public MoveableObject{
private:
	bool hasParticleEffect;
protected:
	Vector2 alteredPos;
public:
	Block(){}

	Block(Vector2 pos){
		numLines = 8;
		lines = new Vector2[numLines];
		radius = 15.0f;

		isOnGround = false;

		position.x = 0;
		position.y = 0;

		lines[0].x = position.x - radius;
		lines[0].y = position.x - radius;
		lines[1].x = position.x + radius;
		lines[1].y = position.y - radius;
		lines[2].x = position.x + radius;
		lines[2].y = position.y - radius;
		lines[3].x = position.x + radius;
		lines[3].y = position.y + radius;
		lines[4].x = position.x + radius;
		lines[4].y = position.y + radius;
		lines[5].x = position.x - radius;
		lines[5].y = position.x + radius;
		lines[6].x = position.x - radius;
		lines[6].y = position.x + radius;
		lines[7].x = position.x - radius;
		lines[7].y = position.x - radius;

		position.x = pos.x;
		position.y = pos.y;

		alteredPos.x = position.x;
		alteredPos.y = position.y;
	}

	virtual void Shift(Vector2 shiftVec){
		position = position + shiftVec;
	}

	void Draw(Core::Graphics& graphics){
		Core::RGB color = RGB(255, 255, 255);
		graphics.SetColor(color);

		_transform = (alteredPos.y < position.y) ? Matrix3::Translation(alteredPos.x, alteredPos.y) : 
			Matrix3::Translation(position.x, position.y);
		DrawObj(graphics);
	}
	bool Update(float dt){
		if (alteredPos.y < position.y){
			velocity.y += (BLOCK_ACCEL * dt);
			alteredPos = alteredPos + velocity;
		}
		else{
			alteredPos.x = position.x;
			alteredPos.y = position.y;
		}

		return true;
	}

	bool HasParticleEffect(){
		return hasParticleEffect;
	}
	virtual Item* Hit(){
		velocity.y = -17.0f;
		alteredPos = alteredPos + velocity;

		return new NullItem(false);
	}
};

#endif