#ifndef MYSTERY_BLOCK_H
#define MYSTERY_BLOCK_H

#include "Block.h"
#include "NullItem.h"
#include "DebugMemory.h"

class MysteryBlock : public Block{
private:
	Item* currentItem;
	bool hasItem, isItemReady;
	typedef Block super;
public:
	MysteryBlock(Vector2 pos, Item* item) : Block(pos){
		currentItem = item;
	}

	void Shift(Vector2 shiftVec){
		if (currentItem->IsInitialized()){
			currentItem->SetPosition(currentItem->GetPosition() + shiftVec);
		}
		super::Shift(shiftVec);
	}
	Item* Hit(){
		Item* result = new NullItem(false);

		if (hasItem){
			delete result;

			result = currentItem;
			hasItem = false;

			super::Hit();
		}

		return result;
	}
	
	void Destroy(){
		if (hasItem){
			currentItem->Destroy();
			delete currentItem;
		}
		delete [] lines;
	}
	
	bool Update(float dt){
		return super::Update(dt);
	}
	void Draw(Core::Graphics& graphics){
		if (!hasItem){
			graphics.DrawLine(position.x - radius, position.y - radius, position.x + radius, position.y + radius);
			graphics.DrawLine(position.x + radius, position.y - radius, position.x - radius, position.y + radius);
		}

		super::Draw(graphics);
	}
};

#endif