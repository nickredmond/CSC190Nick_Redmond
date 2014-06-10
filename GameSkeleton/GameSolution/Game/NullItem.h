#ifndef NULL_ITEM_H
#define NULL_ITEM_H

#include "Item.h"

class NullItem : public Item{
public:
	NullItem(bool init) : Item(init){}

	virtual void Use(Player& plyr){
		plyr;
	}
};

#endif