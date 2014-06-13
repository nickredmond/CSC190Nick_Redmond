#ifndef LEVEL_H
#define LEVEL_H

#include "Koopa.h"
#include "Weapon.h"
#include "Wall.h"
#include "Player.h"
#include "CoinBlock.h"
#include "MysteryBlock.h"
#include "ItemBox.h"
#include "Assertions.h"
#include "DebugMemory.h"

#include <vector>
using std::vector;

const float SHIFT = 4.0f;
const float DEATH_TIME = 2.0f;
const float WIN_TIME = 2.0f;

class Level{
private:
	bool wasFiring, hasWon, hasManager;
	int numberEnemies, numberWalls, numberBlocks, currentBlockIndex, currentEnemyIndex, currentWallIndex, currentItemIndex;
	Wall* walls;
	Enemy* enemies;
	Item** items;
	Block** blocks;
	vector<Weapon*> activeProjectiles;
	Player plyr;
	int leftMargin, rightMargin;
	ParticleManager* manager;
	float timeSinceDeath, timeSinceWin;
	ItemBox winBox;

	void Shift(float shiftAmt){
		Vector2 shiftVec = Vector2(shiftAmt, 0.0f);

		for (int i = 0; i < numberWalls; i++){
			walls[i].v1 = walls[i].v1 + shiftVec;
			walls[i].v2 = walls[i].v2 + shiftVec;
			walls[i].Shift(shiftVec);
		}
		for (int i = 0; i < numberEnemies; i++){
			enemies[i].SetPosition(enemies[i].GetPosition() + shiftVec);
		}
		for (int i = 0; i < numberBlocks; i++){
			blocks[i]->Shift(shiftVec);
		}

		winBox.Shift(shiftVec);

		unsigned int index = 0;
		vector<Weapon*>::iterator iter = activeProjectiles.begin(); 
		while(iter != activeProjectiles.end() && index < activeProjectiles.size()){
			Weapon* w = *iter;
			w->SetPosition(w->GetPosition() + shiftVec);

			iter = activeProjectiles.begin() + index;
			index++;
		}
	}

	bool IsOnScreen(MoveableObject& obj){
		Vector2 pos = obj.GetPosition();
		return (pos.x > 0 && pos.y > 0 && pos.x < screenWidth && pos.y < screenHeight);
	}

	void UpdateWeapons(float dt){
		vector<Weapon*> deletedWeapons;

		for(vector<Weapon*>::iterator iter = activeProjectiles.begin(); 
			iter != activeProjectiles.end(); iter++){
				Weapon* nextEffect = *iter;
				UpdateColors(nextEffect->GetColorChangeType(), nextEffect->GetParticles(), nextEffect->GetNrParticles());
				bool isAlive = nextEffect->Update(dt);
				Collisions::UpdateEnemyWeaponCollisions(*nextEffect, enemies, numberEnemies, *manager);
				Collisions::UpdateWeaponCollisions(*nextEffect, walls, numberWalls, dt);

				if (!isAlive){
					deletedWeapons.push_back(nextEffect);
				}
		}

		for(vector<Weapon*>::iterator iter = deletedWeapons.begin(); 
			iter != deletedWeapons.end(); iter++){
				Weapon* nextEffect = *iter;
				RemoveProjectile(nextEffect);
		}
	}
public:
	int worldNr, levelNr, screenWidth, screenHeight;

	Level(){hasManager = false;}

	Level(int numEnemies, int numWalls, int numBlocks, Player _plyr, int width, ParticleManager* mgr){
		bool isCorrectValues = ((numEnemies > 0) && (numWalls > 0) && (numBlocks > 0));
		ASSERT(isCorrectValues, "Invalid Args :: Level constructor arguments are invalid ('num' specifiers are invalid)")

		walls = new Wall[numWalls];
		enemies = new Enemy[numEnemies];
		blocks = new Block*[numBlocks];
		items = new Item*[numBlocks];

		for (int i = 0; i < numBlocks; i++){
			items[i] = new NullItem(false);
		}

		numberWalls = numWalls;
		numberEnemies = numEnemies;
		numberBlocks = numBlocks;
		currentWallIndex = 0;
		currentEnemyIndex = 0;
		currentBlockIndex = 0;
		currentItemIndex = 0;

		screenWidth = width;
		screenHeight = 0;
		leftMargin = int(0.4f * screenWidth);
		rightMargin = int(0.6f * screenWidth);

		timeSinceDeath = 0.0f;
		timeSinceWin = 0.0f;

		plyr.Destroy();
		plyr = _plyr;
		manager = mgr;
		hasManager = true;

		wasFiring = false;
		hasWon = false;
	}

	void End(){
		plyr.Destroy();
		winBox.Destroy();
		if (hasManager){
			manager->Reset();
		}

		for (int i = 0; i < numberWalls; i++){
			walls[i].Destroy();
		}
		for (int i = 0; i < numberEnemies; i++){
			enemies[i].Destroy();
		}
		for (int i = 0; i < numberBlocks; i++){
			Block* nextBlock = blocks[i];
			nextBlock->Destroy();
			delete nextBlock;
		}
		for (int i = 0; i < numberBlocks; i++){
			Item* nextItem = items[i];
			if (nextItem->IsInitialized() && !nextItem->isNullItem){
				nextItem->Destroy();
			}
			delete nextItem;
		}

		activeProjectiles.clear();
		vector<Weapon*>().swap(activeProjectiles);

		delete [] walls;
		delete [] enemies;
		delete [] blocks;
		delete [] items;
	}

	void SetItemBox(ItemBox box){
		winBox = box;
	}

	bool IsGameOver(){
		return timeSinceDeath >= DEATH_TIME;
	}
	bool IsWin(){
		return timeSinceWin >= WIN_TIME;
	}

	bool Update(float dt){
		plyr.Update(dt);
		UpdateGravity(plyr, dt);

		if (hasWon){
			timeSinceWin += dt;
		}

		if (!plyr.isAlive){
			timeSinceDeath += dt;
		}
		else{
			hasWon = hasWon || Collisions::UpdateItemBoxCollision(plyr, winBox);

			Collisions::UpdateWallCollisions(plyr, walls, numberWalls, dt);
			int itemIncrement = Collisions::UpdateBlockCollisions(plyr, items, currentItemIndex, numberBlocks, blocks, numberBlocks, dt, *manager);
			Collisions::UpdateEnemyCollisions(plyr, enemies, numberEnemies, *manager);
			Collisions::UpdateItemCollisions(plyr, items, numberBlocks);

			currentItemIndex += itemIncrement;
			if (currentItemIndex >= numberBlocks){
				currentItemIndex -= numberBlocks;
			}

			for (int i = 0; i < numberEnemies; i++){
				Enemy* nextEnemy = (enemies + i);

				if (IsOnScreen(*nextEnemy)){
					nextEnemy->Update(dt);
					Collisions::UpdateWallCollisions(*nextEnemy, walls, numberWalls, dt);
					UpdateGravity(*nextEnemy, dt);
				}
			}
			for (int i = 0; i < numberBlocks; i++){
				blocks[i]->Update(dt);
			}
			for (int i = 0; i < numberWalls; i++){
				walls[i].Update(dt);
			}
			for (int i = 0; i < numberBlocks; i++){
				Item* nextItem = items[i];

				if (nextItem->IsInitialized()){
					nextItem->Update(dt);
				}
			}
	
			UpdateWeapons(dt);

			float plyrX = plyr.GetPosition().x;
			float plyrVelX = plyr.GetVelocity().x;

			if (plyrX < leftMargin && plyrVelX < 0.0f){
				plyr.isScrolling = true;
				Shift(-(plyrVelX * dt));
			}
			else if (plyrX > rightMargin && plyrVelX > 0.0f){
				plyr.isScrolling = true;
				Shift(-(plyrVelX * dt));
			}
			else plyr.isScrolling = false;

			if (Input::IsPressed(' ')){
				if (!wasFiring){
					if (plyr._type == ProjectileType::FIREBALL){
						Vector2 weaponPos = plyr.GetPosition() - Vector2(0.0f, 5.0f);

						Weapon* w = new Weapon(0.5f, 0.01f, ColorChangeType::FIRE, weaponPos,
							0.05f, 0.5f, ProjectileType::FIREBALL, plyr.isFacingRight, 250);
						w->SetScreenDimensions(screenWidth, screenHeight);
						AddProjectile(w);
					}
				}

				wasFiring = true;
			}
			else wasFiring = false;

			winBox.Update(dt);

			Vector2 plyrPos = plyr.GetPosition();
			plyr.isAlive = hasWon || (plyr.isAlive && (plyrPos.x > 0 && plyrPos.x < screenWidth && plyrPos.y > 0 &&
				plyrPos.y < screenHeight));
		}

		return (!IsGameOver() && !IsWin());
	}
	void Draw(Core::Graphics& graphics){
		graphics.DrawString(20, 20, "COINS:");
		Debug::DrawValue(graphics, 80, 20, float(plyr.score));

		if (hasWon){
			graphics.DrawString((screenWidth / 2), (screenHeight / 2), "LEVEL COMPLETE");
		}
		else if (!plyr.isAlive){
			graphics.DrawString((screenWidth / 2), (screenHeight / 2), "GAME OVER");
		}

		plyr.Draw(graphics);

		for (int i = 0; i < numberWalls; i++){
			walls[i].Draw(graphics);
		}
		for (int i = 0; i < numberEnemies; i++){
			enemies[i].Draw(graphics);
		}
		for (int i = 0; i < numberBlocks; i++){
			blocks[i]->Draw(graphics);
		}
		for (int i = 0; i < numberBlocks; i++){
			Item* nextItem = items[i];

			if (nextItem->IsInitialized()){
				nextItem->Draw(graphics);
			}
		}

		unsigned int index = 0;
		vector<Weapon*>::iterator iter = activeProjectiles.begin(); 
		while(iter != activeProjectiles.end() && index < activeProjectiles.size()){
			Weapon* w = *iter;
			w->Draw(graphics);
			
			iter = activeProjectiles.begin() + index;

			index++;
		}

		winBox.Draw(graphics);
	}

	bool AddEnemy(Enemy e){
		bool success = currentEnemyIndex < numberEnemies;

		if (success){
			enemies[currentEnemyIndex] = e;
			currentEnemyIndex++;
		}

		return success;
	}
	bool AddWall(Wall w){
		bool success = currentWallIndex < numberWalls;

		if (success){
			walls[currentWallIndex] = w;
			currentWallIndex++;
		}

		return success;
	}
	bool AddBlock(Block* b){
		bool success = currentBlockIndex < numberBlocks;

		if (success){
			blocks[currentBlockIndex] = b;
			currentBlockIndex++;
		}

		return success;
	}

	void AddProjectile(Weapon* p){
		activeProjectiles.push_back(p);
	}
	void RemoveProjectile(int index){
		vector<Weapon*>::iterator removedProj = activeProjectiles.begin() + index;
		activeProjectiles.erase(removedProj);
	}
	void RemoveProjectile(Weapon* p){
		bool found = false;

		unsigned int index = 0;
		vector<Weapon*>::iterator iter = activeProjectiles.begin(); 
		while(iter != activeProjectiles.end() && !found && (index < activeProjectiles.size())){
			if (p == *iter){
				p->Destroy();
				iter = activeProjectiles.erase(iter);
				delete p;

				found = true;
			}
			else iter = activeProjectiles.begin() + index;

			index++;
		}
	}
};

Level GetDefaultLevel(int screenWidth, int screenHeight, ParticleManager* manager){
	Player plyr = Player();

	int numEnemies = 3;
	int numWalls = 4;
	int numBlocks = 2;
	Wall* walls = new Wall[numWalls];
	walls[0] = Wall(Vector2(float(screenWidth / 2.0f), 600.0f), (Vector2(0.0f, 600.0f)));
	walls[1] = Wall(Vector2(float(screenWidth), 500.0f), Vector2(float(screenWidth) / 2.0f, 600.0f));
	
	Wall wall3 = Wall(Vector2(float(screenWidth + 400), 300.0f), Vector2(float(screenWidth), 300.0f), true);
	wall3.AddPathPoint(Vector2(float(screenWidth + 400), 300.0f));
	wall3.AddPathPoint(Vector2(float(screenWidth + 400), 700.0f));
	walls[2] = wall3;

	walls[3] = Wall(Vector2(float(screenWidth + 800), 400.0f), Vector2(float(screenWidth + 400), 500.0f));

	Level lvl = Level(numEnemies, numWalls, numBlocks, plyr, screenWidth, manager);
	lvl.AddWall(walls[0]);
	lvl.AddWall(walls[1]);
	lvl.AddWall(walls[2]);
	lvl.AddWall(walls[3]);

	Item* item = new FireballItem(Vector2(400.0f, 460.0f));

	MysteryBlock* b = new MysteryBlock(Vector2(400.0f, 500.0f), item);
	lvl.AddBlock(b);
	CoinBlock* cb = new CoinBlock(Vector2(450.0f, 500.0f));
	lvl.AddBlock(cb);

	Enemy e = Koopa();
	e.SetPosition(Vector2(600.0f, 300.0f));
	lvl.AddEnemy(e);

	Enemy e2 = Koopa();
	e2.SetPosition(Vector2(1200.0f, 300.0f));
	lvl.AddEnemy(e2);

	Enemy e3 = Koopa();
	e3.SetPosition(Vector2(1500.0f, 300.0f));
	lvl.AddEnemy(e3);

	ItemBox box = ItemBox(Vector2(float(screenWidth + 800), 300.0f));
	lvl.SetItemBox(box);

	lvl.screenWidth = screenWidth;
	lvl.screenHeight = screenHeight;

	delete [] walls;

//	LOG(Info, "Initialized default level.");
	return lvl;
}

#endif