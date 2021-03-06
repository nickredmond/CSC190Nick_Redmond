#ifndef COIN_BLOCK_H
#define COIN_BLOCK_H

#include "Block.h"
#include "Coin.h"
#include "Item.h"
#include "DebugMemory.h"

class CoinBlock : public Block{
private:
	Coin currentCoin;
	bool hasCoin;
	typedef Block super;
	int numberCoins;
public:
	CoinBlock(Vector2 pos) : Block(pos){
		hasCoin = false;
		numberCoins = 3;
	}

	bool HasCoins(){
		return numberCoins > 0;
	}
	Item* Hit(){
		bool initialized = HasCoins();

		if (numberCoins > 0){
			numberCoins--;
			currentCoin = Coin(position);
			hasCoin = true;

			Item* temp = super::Hit();
			delete temp;
		}

		Item* coin = new NullItem(initialized);
		coin->isCoin = true;
		return coin;
	}
	
	bool Update(float dt){
		if (hasCoin){
			if (currentCoin.IsAlive()){
				currentCoin.Update(dt);
			}
			else hasCoin = false;
		}

		return super::Update(dt);
	}
	void Draw(Core::Graphics& graphics){
		if (!HasCoins()){
			graphics.DrawLine(position.x - radius, position.y - radius, position.x + radius, position.y + radius);
			graphics.DrawLine(position.x + radius, position.y - radius, position.x - radius, position.y + radius);
		}
		if (hasCoin){
			currentCoin.Draw(graphics);
		}

		super::Draw(graphics);
	}
};

#endif