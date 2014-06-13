#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "Player.h"
#include "Enemy.h"
#include "Wall.h"
#include "Block.h"
#include "ParticleManager.h"
#include "ExplosionEffect.h"
#include "Weapon.h"
#include "ItemBox.h"
#include "Assertions.h"
#include "DebugMemory.h"

using Obstacles::Wall;

namespace Collisions{
	bool UpdateItemBoxCollision(Player& plyr, ItemBox& box){
		bool isCollision = (box.GetPosition() - plyr.GetPosition()).length() < box.radius;

		if (isCollision){
			Item* item = box.Hit();item;
		}

		return isCollision;
	}

	void UpdateWallCollisions(MoveableObject& obj, Wall* walls, int numWalls, float dt){
		obj.isOnGround = false;

		for (int i = 0; i < numWalls; i++){
			Wall nextWall = *(walls+i);

			if(IsCollision(obj, dt, nextWall.v1, nextWall.v2)){
				Vector2 groundVel = (obj.GetVelocity().x > 0.0f) ? (nextWall.v2 - nextWall.v1) : (nextWall.v1 - nextWall.v2);
				Vector2 objVel = obj.GetVelocity().length() * groundVel.normalized();

				obj.isOnGround = true;
				obj.SetVelocity(obj.GetVelocity().x, -objVel.y);

				float objX = obj.GetPosition().x;
				float lineRatio = (objX - nextWall.v1.x) / (nextWall.v2.x - nextWall.v1.x);

				float yVal = nextWall.v1.y + (lineRatio * (nextWall.v2.y - nextWall.v1.y));yVal;

				if (nextWall.IsLerping()){
					obj.SetPosition(Vector2(objX, (yVal - obj.radius)));
				}
			}
		}
	}

	void UpdateWeaponCollisions(Weapon& w, Wall* walls, int numWalls, float dt){
		MoveableObject obj = MoveableObject();
		Vector2 pos = w.GetPosition();
		obj.SetPosition(pos);
		obj.radius = WEAPON_RADIUS;

		obj.isOnGround = false;

		for (int i = 0; i < numWalls; i++){
			Wall nextWall = *(walls+i);

			if(IsCollision(obj, dt, nextWall.v1, nextWall.v2)){
				Vector2 groundVel = (obj.GetVelocity().x > 0.0f) ? (nextWall.v2 - nextWall.v1) : (nextWall.v1 - nextWall.v2);
				Vector2 objVel = obj.GetVelocity().length() * groundVel.normalized();

				obj.isOnGround = true;
				w.Bounce();
			}
		}
	}

	int UpdateBlockCollisions(Player& obj, Item** items, int currentItemIndex, int numberItems, Block** blocks, int numBlocks, float dt,
		ParticleManager& manager){
		dt;
		bool isValidIndex = (currentItemIndex < numberItems && currentItemIndex >= 0);
		bool isValidNrItems = (numberItems <= numBlocks);
		ASSERT(isValidIndex, "Index OutOfBounds :: currentItemIndex is not in valid range.");
		ASSERT(isValidNrItems, "Invalid Argument :: numberItems cannot be larger than numBlocks.");

		int itemIndex = currentItemIndex;
		int itemIncrement = 0;

		for (int i = 0; i < numBlocks; i++){
			Block* nextBlock = *(blocks+i);

			bool isCollision = (nextBlock->GetPosition() - obj.GetPosition()).length() < obj.radius;
			bool isUnder = obj.GetPosition().y > nextBlock->GetPosition().y;
			bool isRising = obj.GetVelocity().y < 0.0f;

			if (isCollision && isUnder && isRising){
				obj.SetVelocity(obj.GetVelocity().x, -obj.GetVelocity().y);
				Item* item = nextBlock->Hit();
				manager;

				if (item->IsInitialized()){
					delete items[itemIndex];
					items[itemIndex] = item;
					itemIndex = (itemIndex < numberItems - 1) ? itemIndex + 1 : 0;
					itemIncrement++;

					if (item->isCoin){
						obj.score = obj.score + 1;
					}
				}
				else delete item;
			}
		}

		return itemIncrement;
	}

	void UpdateEnemyCollisions(Player& plyr, Enemy* enemies, int numEnemies,
		ParticleManager& manager){
		for (int i = 0; i < numEnemies; i++){
			Enemy* nextEnemy = (enemies+i);

			float distance = (plyr.GetPosition() - nextEnemy->GetPosition()).length();
			bool isCollision = (distance < nextEnemy->radius);
			
			if (isCollision){
				if (nextEnemy->IsAlive()){
					ParticleEffect* explosion = new ExplosionEffect(0.15f, 0.1f, ColorChangeType::FIRE,
						nextEnemy->GetPosition(), 1.0f, 10.0f, 215);
					manager.AddEffect(explosion);

					plyr.Hurt();
				}

				nextEnemy->Hurt();
			}
		}
	}

	void UpdateItemCollisions(Player& plyr, Item** items, int numItems){
		for (int i = 0; i < numItems; i++){
			Item* nextItem = *(items+i);

			float distance = (plyr.GetPosition() - nextItem->GetPosition()).length();
			bool isCollision = (distance < nextItem->radius);
			
			if (isCollision){
				if (nextItem->IsInitialized()){
					nextItem->Use(plyr);
				}
			}
		}
	}

	void UpdateEnemyWeaponCollisions(Weapon& w, Enemy* enemies, int numEnemies,
		ParticleManager& manager){
		for (int i = 0; i < numEnemies; i++){
			Enemy* nextEnemy = (enemies+i);

			float distance = (w.GetPosition() - nextEnemy->GetPosition()).length();
			bool isCollision = (distance < nextEnemy->radius);
			
			if (isCollision){
				if (nextEnemy->IsAlive()){
					ParticleEffect* explosion = new ExplosionEffect(0.15f, 0.1f, ColorChangeType::FIRE,
						nextEnemy->GetPosition(), 1.0f, 10.0f, 215);
					manager.AddEffect(explosion);

					w.SetPosition(Vector2(-10.0f, -10.0f));
				}

				nextEnemy->Hurt();

			}
		}
	}
}

#endif