#ifndef ITEM_H
#define ITEM_H

#include "GameObject.h"
#include "Player.h"

class Item : public MoveableObject{
protected:
	bool initialized;
public:
	bool isCoin, isNullItem;

	Item(){
		isCoin = false;
		initialized = false;
		isNullItem = false;
	}
	Item(bool init){
		isCoin = false;
		initialized = init;
	}

	bool IsInitialized(){
		return initialized;
	}

	virtual void Use(Player& plyr) = 0;
};

#endif