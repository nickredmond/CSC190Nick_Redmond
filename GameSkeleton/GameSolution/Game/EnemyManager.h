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
	Bullet* shipBullets;
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

	bool IsCollision(Vector2 pos, Vector2 shipVel, MoveableObject& enemy, float dt, float collisionRadius){
		dt;
		shipVel;

		Vector2 shipMinusEnemy = pos - enemy.GetPosition();
		return (shipMinusEnemy.length() <= collisionRadius);
	}

	void DetectShipBulletCollisions(float dt){
		for (int i = 0; i < MAX_BULLET_INDEX + 1; i++){
			for (int j = 0; j < chaserIndex; j++){
				if (chasers[j].IsAlive() && !shipBullets[i].isNull && IsCollision(shipBullets[i].GetPosition(), Vector2(), chasers[j], dt, chasers[j].collisionRadius)){
					chasers[j].Kill();
					shipBullets[i].isVisible = false;
				
					ParticleEffect* explosion = new ExplosionEffect(0.15f, 0.1f, ColorChangeType::RANDOM,
						chasers[j].GetPosition(), 1.0f, 10.0f, 215);
					particles.AddEffect(explosion);
				}
			}
			for (int j = 0; j < turretIndex; j++){
				if (turrets[j].IsAlive() && !shipBullets[i].isNull && IsCollision(shipBullets[i].GetPosition(), Vector2(), turrets[j], dt, turrets[j].collisionRadius)){
					turrets[j].Kill();
					shipBullets[i].isVisible = false;
				
					ParticleEffect* explosion = new ExplosionEffect(0.15f, 0.1f, ColorChangeType::RANDOM,
						turrets[j].GetPosition(), 1.0f, 10.0f, 215);
					particles.AddEffect(explosion);
				}
			}
		}
	}
public:
	EnemyManager(){}

	EnemyManager(ParticleManager& manager, Bullet* _shipBulets, int mothershipKills, float _spawnRate, int _screenWidth, int _screenHeight, int _maxTurrets = 5,
		int _maxChasers = 10){
		particles = manager;
		shipBullets = _shipBulets;

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

	void Update(Spaceship& ship, float dt){
		timeSinceSpawn += dt;

		if (timeSinceSpawn >= spawnRate){
			SpawnEnemy();
		}

		for (int i = 0; i < turretIndex; i++){
			turrets[i].Update(ship.GetPosition(), dt);

			if (turrets[i].CanFire()){
				Bullet* firedBullets = turrets[i].Fire();

				for (int j = 0; j < turrets[i].GetBulletsPerShot(); j++){
					bullets[bulletIndex] = *(firedBullets + j);
					bullets[bulletIndex].isVisible = true;
					bulletIndex++;

					if (bulletIndex >= MAX_BULLETS){
						bulletIndex = 0;
						hasFilledBullets = true;
					}
				}
			}
		}
		for (int i = 0; i < chaserIndex; i++){
			if (chasers[i].IsAlive()){
				chasers[i].Update(dt, ship.GetPosition());

				if (IsCollision(ship.GetPosition(), ship.GetVelocity(), chasers[i], dt, chasers[i].collisionRadius)){
					chasers[i].Kill();
				
					ParticleEffect* explosion = new ExplosionEffect(0.15f, 0.1f, ColorChangeType::RANDOM,
						chasers[i].GetPosition(), 1.0f, 10.0f, 215);
					particles.AddEffect(explosion);
				}
			}
		}
		// draw mothership

		int currentMaxIndex = (hasFilledBullets ? MAX_BULLETS : bulletIndex);
		for (int i = 0; i < currentMaxIndex; i++){
			bullets[i].Update(dt);

			if (IsCollision(bullets[i].GetPosition(), Vector2(0, 0), ship, dt, 12.5f)){
				bullets[i].isVisible = false;
			}
		}

		DetectShipBulletCollisions(dt);

		particles.Update(true, 0.0f, ship.GetPosition(), dt);
	}
	void Draw(Core::Graphics& graphics){
		Core::RGB color = RGB(255, 255, 255);
		graphics.SetColor(color);

		for (int i = 0; i < turretIndex; i++){
			if (turrets[i].IsAlive()){
				turrets[i].Draw(graphics);
			}
		}
		for (int i = 0; i < chaserIndex; i++){
			if (chasers[i].IsAlive()){
				chasers[i].Draw(graphics);
			}
		}

		int currentMaxIndex = (hasFilledBullets ? MAX_BULLETS : bulletIndex);
		for (int i = 0; i < currentMaxIndex; i++){
			if (bullets[i].isVisible){
				bullets[i].Draw(graphics);
			}
		}

		particles.Draw(graphics);
	}
};

#endif