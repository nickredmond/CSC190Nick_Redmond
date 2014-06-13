#ifndef NULL_ITEM_H
#define NULL_ITEM_H

#include "Item.h"

class NullItem : public Item{
public:
	NullItem(bool init) : Item(init){
		isNullItem = true;
	}

	virtual void Use(Player& plyr){
		plyr;
	}
};

#endif