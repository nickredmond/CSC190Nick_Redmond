#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "AutoTurret.h"
#include "Utils.h"
#include "ParticleManager.h"
#include "ExplosionEffect.h"

const float WALL_BUFFER = 5.0f;
const float TURRET_SPAWN_WIDTH = 0.1f;
const int MAX_BULLETS = 150;

class EnemyManager{
private:
	AutoTurret* turrets;
	Asteroid* chasers;
	// Mothership mother;
	int killsBeforeMothership, killsSinceMothership;
	int turretIndex, maxTurrets, chaserIndex, maxChasers, bulletIndex;
	float spawnRate, timeSinceSpawn;
	int screenWidth, screenHeight;
	Bullet* bullets;
	bool hasFilledBullets;
	ParticleManager particles;

	Vector2 RandomTurretPosition(int width, int height){
		float perimeterRatio = Utils::randomFloat();
		float perimeter = Utils::Perimeter(float(width), float(height));

		float xRatio = float(width) / perimeter;
		float yRatio = float(height) / perimeter;
		
		float xVal = 0.0f;
		float yVal = 0.0f;
		
		if (perimeterRatio < xRatio){ // top
			xVal = Utils::randomInRange(WALL_BUFFER, width - WALL_BUFFER);
			yVal = Utils::randomInRange(WALL_BUFFER, (height * TURRET_SPAWN_WIDTH));
		}
		else if (perimeterRatio < (xRatio + yRatio)){ // right
			xVal = Utils::randomInRange((width - (width * TURRET_SPAWN_WIDTH)), width - WALL_BUFFER);
			yVal = Utils::randomInRange(WALL_BUFFER, height - WALL_BUFFER);
		}
		else if (perimeterRatio < ((2 * xRatio) + yRatio)){ // bottom
			xVal = Utils::randomInRange(WALL_BUFFER, width - WALL_BUFFER);
			yVal = Utils::randomInRange(height - (height * TURRET_SPAWN_WIDTH), height - WALL_BUFFER);
		}
		else{ // left
			xVal = Utils::randomInRange(WALL_BUFFER, (width * TURRET_SPAWN_WIDTH));
			yVal = Utils::randomInRange(WALL_BUFFER, height - WALL_BUFFER);
		}

		return Vector2(xVal, yVal);
	}

	void SpawnEnemy(){
		timeSinceSpawn = 0.0f;

		int enemyIndex = int(Utils::randomFloat() * 2);

		if (enemyIndex == 0){
			if (turretIndex < maxTurrets){
				turrets[turretIndex] = AutoTurret("Single Shot", RandomTurretPosition(screenWidth, screenHeight));
				turretIndex++;
			}
			else{
				bool foundSpot = false;

				for (int i = 0; i < maxTurrets && !foundSpot; i++){
					if (!turrets[i].IsAlive()){
						turrets[i] = AutoTurret("Single Shot", RandomTurretPosition(screenWidth, screenHeight));
						foundSpot = true;
					}
				}
			}
		}
		else{
			if (chaserIndex < maxChasers){
				chasers[chaserIndex] = Asteroid(RandomTurretPosition(screenWidth, screenHeight), false); // change from rand turret pos?
				chaserIndex++;
			}
			else{
				bool foundSpot = false;

				for (int i = 0; i < maxChasers && !foundSpot; i++){
					if (!chasers[i].IsAlive()){
						chasers[i] = Asteroid(RandomTurretPosition(screenWidth, screenHeight), false);
						foundSpot = true;
					}
				}
			}
		}
	}

	Vector2 normalizedPerp(Vector2 v1, Vector2 v2){
		Vector2 p = v2 - v1;
		p = p.normalized();
		return p.perpCCW();
	}

	bool IsCollision(Vector2 shipPos, Vector2 shipVel, MoveableObject& enemy, float dt){
		dt;
		shipVel;

		float collisionRadius = 5.0f;
		Vector2 shipMinusEnemy = shipPos - enemy.GetPosition();

		return (shipMinusEnemy.length() <= collisionRadius);

		//for (int i = 0; i < enemy.numLines && !collision; i+=2){
		//	Vector2 n = normalizedPerp(enemy.lines[i], enemy.lines[i+1]);

		//	Vector2 q1 = shipPos - enemy.lines[i];
		//	float d1 = q1.dotProduct(n);

		//	Vector2 afterPos = shipPos + (shipVel * dt);
		//	Vector2 q2 = afterPos - enemy.lines[i];
		//	float d2 = q2.dotProduct(n);

		//	collision = (d1 > 0 && d2 < 0);
		//}

		//return collision;
	}
public:
	EnemyManager(ParticleManager& manager, int mothershipKills, float _spawnRate, int _screenWidth, int _screenHeight, int _maxTurrets = 5,
		int _maxChasers = 10){
		particles = manager;

		killsBeforeMothership = mothershipKills;
		spawnRate = _spawnRate;

		killsSinceMothership = 0;
		timeSinceSpawn = 0.0f;
		turretIndex = 0;
		chaserIndex = 0;

		maxTurrets = _maxTurrets;
		turrets = new AutoTurret[maxTurrets];
		maxChasers = _maxChasers;
		chasers = new Asteroid[maxChasers];

		screenWidth = _screenWidth;
		screenHeight = _screenHeight;

		bulletIndex = 0;
		bullets = new Bullet[MAX_BULLETS];
	}
	//~EnemyManager(){
	//	delete [] turrets;
	//}

	void Update(Vector2 shipPos, Vector2 shipVel, float dt){
		timeSinceSpawn += dt;

		if (timeSinceSpawn >= spawnRate){
			SpawnEnemy();
		}

		for (int i = 0; i < turretIndex; i++){
			turrets[i].Update(shipPos, dt);

			if (turrets[i].CanFire()){
				Bullet* firedBullets = turrets[i].Fire();

				for (int j = 0; j < turrets[i].GetBulletsPerShot(); j++){
					bullets[bulletIndex] = *(firedBullets + j);
					bulletIndex++;

					if (bulletIndex >= MAX_BULLETS){
						bulletIndex = 0;
						hasFilledBullets = true;
					}
				}
			}
		}
		for (int i = 0; i < chaserIndex; i++){
			chasers[i].Update(dt, shipPos);

			if (IsCollision(shipPos, shipVel, chasers[i], dt) && chasers[i].IsAlive()){
				chasers[i].Kill();
				
				ParticleEffect* explosion = new ExplosionEffect(0.15f, 0.1f, ColorChangeType::FIRE,
				shipPos, 1.0f, 10.0f, 215);
				particles.AddEffect(explosion);
			}
		}
		// draw mothership

		int currentMaxIndex = (hasFilledBullets ? MAX_BULLETS : bulletIndex);
		for (int i = 0; i < currentMaxIndex; i++){
			bullets[i].Update(dt);
		}

		particles.Update(true, 0.0f, shipPos, dt);
	}
	void Draw(Core::Graphics& graphics){
		Core::RGB color = RGB(255, 255, 255);
		graphics.SetColor(color);

		for (int i = 0; i < turretIndex; i++){
			turrets[i].Draw(graphics);
		}
		for (int i = 0; i < chaserIndex; i++){
			chasers[i].Draw(graphics);
		}

		int currentMaxIndex = (hasFilledBullets ? MAX_BULLETS : bulletIndex);
		for (int i = 0; i < currentMaxIndex; i++){
			bullets[i].Draw(graphics);
		}

		particles.Draw(graphics);
	}
};

#endif