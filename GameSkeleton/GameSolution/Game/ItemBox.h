#ifndef ITEM_BOX_H
#define ITEM_BOX_H

#include "FireballItem.h"
#include "GameObject.h"
#include "DebugMemory.h"

const int NUMBER_ITEMS = 1;
const float ITEM_DECCEL = 0.05f;
const float ITEM_VEL = -10.0f;

class ItemBox : public MoveableObject{
private:
	Item* items[NUMBER_ITEMS];
	int currentItemIndex, numberItems;
	bool isHit;
	Vector2 velocity;
public:
	ItemBox(){
		numberItems = 0;
	}

	ItemBox(Vector2 pos){
		isHit = false;
		position = Vector2();
		velocity = Vector2(0.0f, ITEM_VEL);
		radius = 40.0f;

		numLines = 8;
		lines = new Vector2[numLines];

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

		currentItemIndex = 0;
		numberItems = NUMBER_ITEMS;
		items[0] = new FireballItem(position);
	}

	void Destroy(){
		for (int i = 0; i < numberItems; i++){
			if (items[i]->IsInitialized()){
				items[i]->Destroy();
			}
			delete items[i];
		}
		delete [] lines;
	}

	void Shift(Vector2 shiftVec){
		position = position + shiftVec;

		for (int i = 0; i < NUMBER_ITEMS; i++){
			items[i]->SetPosition(items[i]->GetPosition() + shiftVec);
		}
	}

	Item* Hit(){
		isHit = true;
		return items[currentItemIndex];
	}
	bool Update(float dt){
		dt;
		_transform = Matrix3::Translation(position);
		items[currentItemIndex]->Update(dt);

		if (isHit){
			velocity = velocity + Vector2(0.0f, ITEM_DECCEL);
			items[currentItemIndex]->SetPosition(items[currentItemIndex]->GetPosition() + velocity);
		}

		return true;
	}
	void Draw(Core::Graphics& graphics){
		Core::RGB color = RGB(255, 255, 255);
		graphics.SetColor(color);

		items[currentItemIndex]->Draw(graphics);
		DrawObj(graphics);
	}
};

#endif