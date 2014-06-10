#ifndef ITEM_H
#define ITEM_H

#include "GameObject.h"
#include "Player.h"

class Item : public MoveableObject{
protected:
	bool initialized;
public:
	Item(){
		initialized = false;
	}
	Item(bool init){
		initialized = init;
	}
	bool IsInitialized(){
		return initialized;
	}

	virtual void Use(Player& plyr) = 0;
};

#endif