#ifndef UPDATE_H
#define UPDATE_H

#include "Spaceship.h"
#include "Utils.h"
#include "Particle.h"
#include "Wall.h"

using namespace Obstacles;

bool wasMousePressed;
int currentBulletIndex;
Bullet* bullets;

const int MAX_BULLET_INDEX = 49;
const int MAX_BG_INDEX = 3;

HierarchialTransform bgObjects[4];

typedef bool (*UpdateShip_CheckEnvironment)(Spaceship& ship, int screenWidth, int screenHeight, float dt);
UpdateShip_CheckEnvironment shipUpdateFn;

char* updateBehaviorTitle;
char* currentWeaponTitle;

bool Update_ScreenWrap(Spaceship& ship, int screenWidth, int screenHeight, float dt){
	dt;
	Vector2 position = ship.GetPosition();
	float newX = position.x;
	float newY = position.y;
	
	if (ship.IsOffRight(screenWidth)){
		newX = float(ship._width) / 2;
	}
	else if (ship.IsOffLeft()){
		newX = screenWidth - (float(ship._width) / 2);
	}

	if (ship.IsOffBottom(screenHeight)){
		newY = float(ship._height) / 2;
	}
	else if (ship.IsOffTop()){
		newY = screenHeight - (float(ship._height) / 2);
	}

	ship.SetPosition(Vector2(newX, newY), Vector2(float(Input::GetMouseX()), float(Input::GetMouseY())), dt);
	return false;
}

bool Update_Bounce(Spaceship& ship, int screenWidth, int screenHeight, float dt){
	bool isBounce = false;

	dt;
	Vector2 position = ship.GetPosition();
	float newXvel = ship.GetVelocity().x;
	float newYvel = ship.GetVelocity().y;

	if (ship.IsOffRight(screenWidth) || ship.IsOffLeft()){
		isBounce = true;
		newXvel = -newXvel;
	}
	if (ship.IsOffBottom(screenHeight) || ship.IsOffTop()){
		isBounce = true;
		newYvel = -newYvel;
	}

	ship.SetVelocity(newXvel, newYvel);
	return isBounce;
}

bool Update_Obstacles(Spaceship& ship, int screenWidth, int screenHeight, float dt){
	Update_ScreenWrap(ship, screenWidth, screenHeight, dt);
	bool isBounce = false;

	Wall* walls = GetWalls();
	Wall wall1 = walls[0];
	Wall wall2 = walls[1];
	Wall wall3 = walls[2];
	Wall wall4 = walls[3];

	if (IsCollision(ship, wall1, dt)){
		isBounce = true;
		RespondToCollisions(ship, wall1);
	}
	if (IsCollision(ship, wall2, dt)){
		isBounce = true;
		RespondToCollisions(ship, wall2);
	}
	if (IsCollision(ship, wall3, dt)){
		isBounce = true;
		RespondToCollisions(ship, wall3);
	}
	if (IsCollision(ship, wall4, dt)){
		isBounce = true;
		RespondToCollisions(ship, wall4);
	}

	//for (int i = 0; i < (sizeof(walls) / sizeof(*walls)); i++){
	//	if (IsCollision(ship, *(walls+i), dt)){
	//		RespondToCollisions(ship, *(walls+i));
	//	}
	//}

	return isBounce;
}

void UpdateShipBehavior(){
	if (Input::IsPressed(VK_F1)){
		updateBehaviorTitle = "CURRENT MODE: Screen Wrap";
		shipUpdateFn = Update_ScreenWrap;
	}
	else if (Input::IsPressed(VK_F2)){
		updateBehaviorTitle = "CURRENT MODE: Bounce";
		shipUpdateFn = Update_Bounce;
	}
	else if (Input::IsPressed(VK_F3)){
		updateBehaviorTitle = "CURRENT MODE: Wall Collisions";
		shipUpdateFn = Update_Obstacles;
	}
}

void UpdateSelectedWeapon(Spaceship& ship){
	if (Input::IsPressed('1')){
		ship.SetTurret("Single Shot");
		currentWeaponTitle = "CURRENT WEAPON: Single Shot";
	}
	else if (Input::IsPressed('2')){
		ship.SetTurret("Triple Shot");
		currentWeaponTitle = "CURRENT WEAPON: Triple Shot";
	}
	else if (Input::IsPressed('3')){
		ship.SetTurret("Laser");
		currentWeaponTitle = "CURRENT WEAPON: Laser";
	}
}

void UpdateShipPosition(Spaceship& ship, float dt){

	Vector2 velocity = ship.GetVelocity();

	if (Input::IsPressed('A')){
		ship.angle += ship.omega;
	}
	if (Input::IsPressed('D')){
		ship.angle -= ship.omega;
	}

	if (Input::IsPressed('W')){
		Vector2 deltaVel = (Matrix3::Rotation(ship.angle + 3.14159265f) * Vector2(0, ACCELERATION));
		velocity.x -= deltaVel.x;
		velocity.y += deltaVel.y;
	}
	else if (Input::IsPressed('S')){
		Vector2 deltaVel = (Matrix3::Rotation(ship.angle) * Vector2(0, ACCELERATION));
		velocity.x -= deltaVel.x;
		velocity.y += deltaVel.y;
	}
	else{
		float xVelInc = 0.0f;
		float yVelInc = 0.0f;

		if (ship.GetVelocity().y > 0){
			yVelInc = -ACCELERATION / 2;
		}
		else if (ship.GetVelocity().y < 0){
			yVelInc = ACCELERATION / 2;
		}
		if (ship.GetVelocity().x > 0){
			xVelInc = -ACCELERATION / 2;
		}
		else if (ship.GetVelocity().x < 0){
			xVelInc = ACCELERATION / 2;
		}

		velocity = velocity + Vector2(xVelInc, yVelInc);
	}

	ship.SetVelocity(velocity.x, velocity.y);

	int x = Input::GetMouseX();
	int y = Input::GetMouseY();

	Vector2 newpos = ship.GetPosition() + (velocity * dt);
	ship.SetPosition(newpos, Vector2(float(x), float(y)), dt);

	if (Input::IsPressed(Input::BUTTON_LEFT) && !wasMousePressed && !ship.GetTurret().isReloading && ship.GetTurret().CanFire()){
		Bullet* newBullets = ship.GetTurret().Fire();

		for (int i = 0; i < ship.GetTurret().GetBulletsPerShot(); i++){
			bullets[currentBulletIndex] = *(newBullets+i);
			bullets[currentBulletIndex].isVisible = true;
			currentBulletIndex++;

			if (currentBulletIndex > MAX_BULLET_INDEX){
				currentBulletIndex = 0;
			}
		}
	}
}

void UpdateBullets(float dt){
	for (int i = 0; i <= MAX_BULLET_INDEX; i++){
		if (bullets[i].isVisible){
			bullets[i].Update(dt);
		}
	}
}

void SetupBgObjects(){
	bgObjects[0] = HierarchialTransform(0.8f, 50.0f, 3, Vector2(100, 100));;
	bgObjects[1] = HierarchialTransform(0.8f, 50.0f, 3, Vector2(700, 100));;
	bgObjects[2] = HierarchialTransform(0.8f, 50.0f, 3, Vector2(700, 500));;
	bgObjects[3] = HierarchialTransform(0.8f, 50.0f, 3, Vector2(100, 500));;
}

void UpdateColors(ColorChangeType type, Particle* particles, int numParticles){
	for (int i = 0; i < numParticles; i++){
		int redValue = 255;
		int greenValue = 255;
		int blueValue = 255;
		Particle* p = (particles + i);
		float ageRatio = p->GetAge() / p->GetLifetime();

		if (type == ColorChangeType::BUBBLE){
			float blueDecreaseRatio = 0.8f;

			float blueRatio = 1.0f;

			if (ageRatio > blueDecreaseRatio){
				blueRatio = (1.0f - ageRatio) / (1.0f - blueDecreaseRatio);
			}

			redValue = int((1.0f - ageRatio) * 200.0f);
			greenValue = int((1.0f - ageRatio) * 200.0f);
			blueValue = int(blueRatio * 255);
		}
		else if (type == ColorChangeType::FIRE){
			// white > yellow > orange > red > black
			
			// 255, 255, 255
			// 255, 255, 0
			// 255, 0, 0
			// 0, 0, 0

			if (ageRatio < 0.33f){
				float blueRatio = 1.0f - (ageRatio / 0.33f);
				blueValue = int(255 * blueRatio);
			}
			else if (ageRatio >= 0.33f && ageRatio < 0.67f){
				float greenRatio = 1.0f - ((ageRatio - 0.33f) / 0.34f);
				greenValue = int(255 * greenRatio);

				blueValue = 0;
			}
			else if (ageRatio < 1.0f){
				float redRatio = 1.0f - ((ageRatio - 0.67f) / 0.33f);
				redValue = int(255 * redRatio);

				blueValue = 0;
				greenValue = 0;
			}
		}
		else if (type == ColorChangeType::RANDOM){
			redValue = p->_r - int(Utils::randomInRange(0.0f, 3.0f));
			greenValue = p->_g - int(Utils::randomInRange(0.0f, 3.0f));
			blueValue = p->_b - int(Utils::randomInRange(0.0f, 3.0f));
		}
		else if (type == ColorChangeType::EXPLOSION){
			// NOT YET IMPLEMENTED
		}

		p->SetColor(redValue, greenValue, blueValue);
	}
}

#endif