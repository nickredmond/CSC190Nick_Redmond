#include "Spaceship.h"
#include "Wall.h"
#include "Asteroid.h"
#include "Debug.h"
#include "HierarchialTransform.h"

using Core::Input;
using namespace Obstacles;

typedef void (*UpdateShip_CheckEnvironment)(Spaceship& ship, int screenWidth, int screenHeight, float dt);
UpdateShip_CheckEnvironment shipUpdateFn;
char* updateBehaviorTitle;
char* currentWeaponTitle;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

Spaceship ship(20, 20);
Asteroid asteroid(Vector2(50, 30));
Bullet bullets[50];
HierarchialTransform bgObjects[4];

int currentBulletIndex;
const int MAX_BULLET_INDEX = 49;
const int MAX_BG_INDEX = 3;

bool wasMousePressed;
Core::RGB defaultColor = RGB(255, 255, 255);

// -- util functions -- //

bool IsOffRight(Vector2 position, Spaceship& ship, int screenWidth){
	return ((position.x + (ship._width / 2) > screenWidth) && ship.GetVelocity().x > 0);
}
bool IsOffLeft(Vector2 position, Spaceship& ship){
	return ((position.x - (ship._width / 2) < 0) && ship.GetVelocity().x < 0);
}
bool IsOffTop(Vector2 position, Spaceship& ship){
	return ((position.y < 0) && ship.GetVelocity().y < 0);
}
bool IsOffBottom(Vector2 position, Spaceship& ship, int screenHeight){
	return ((position.y + (ship._height / 2) > screenHeight) && ship.GetVelocity().y > 0);
}

// -- update functions -- //

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

void Update_ScreenWrap(Spaceship& ship, int screenWidth, int screenHeight, float dt){
	dt;
	Vector2 position = ship.GetPosition();
	float newX = position.x;
	float newY = position.y;

	if (IsOffRight(position, ship, screenWidth)){
		newX = float(ship._width) / 2;
	}
	else if (IsOffLeft(position, ship)){
		newX = screenWidth - (float(ship._width) / 2);
	}

	if (IsOffBottom(position, ship, screenHeight)){
		newY = float(ship._height) / 2;
	}
	else if (IsOffTop(position, ship)){
		newY = screenHeight - (float(ship._height) / 2);
	}

	ship.SetPosition(Vector2(newX, newY), Vector2(float(Input::GetMouseX()), float(Input::GetMouseY())), dt);
}

void Update_Bounce(Spaceship& ship, int screenWidth, int screenHeight, float dt){
	dt;
	Vector2 position = ship.GetPosition();
	float newXvel = ship.GetVelocity().x;
	float newYvel = ship.GetVelocity().y;

	if (IsOffRight(position, ship, screenWidth) || IsOffLeft(position, ship)){
		newXvel = -newXvel;
	}
	if (IsOffBottom(position, ship, screenHeight) || IsOffTop(position, ship)){
		newYvel = -newYvel;
	}

	ship.SetVelocity(newXvel, newYvel);
}

void Update_Obstacles(Spaceship& ship, int screenWidth, int screenHeight, float dt){
	Update_ScreenWrap(ship, screenWidth, screenHeight, dt);

	Wall* walls = GetWalls();
	Wall wall1 = walls[0];
	Wall wall2 = walls[1];
	Wall wall3 = walls[2];
	Wall wall4 = walls[3];

	if (IsCollision(ship, wall1, dt)){
		RespondToCollisions(ship, wall1);
	}
	if (IsCollision(ship, wall2, dt)){
		RespondToCollisions(ship, wall2);
	}
	if (IsCollision(ship, wall3, dt)){
		RespondToCollisions(ship, wall3);
	}
	if (IsCollision(ship, wall4, dt)){
		RespondToCollisions(ship, wall4);
	}

	//for (int i = 0; i < NUM_WALLS; i++){
	//	Wall nextWall = walls[i];

	//	if (IsCollision(ship, nextWall, dt)){
	//		RespondToCollisions(ship, nextWall);
	//	}
	//}
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

void UpdateSelectedWeapon(){
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

//void UpdateTurret(float dt){
//	//Turret gun = ship.GetTurret();
//
//	//if (!gun.isReloading){
//	//	Bullet = gun.Fire();
//	//}
//}

void UpdateBullets(float dt){
	for (int i = 0; i <= MAX_BULLET_INDEX; i++){
		if (bullets[i].isVisible){
			bullets[i].Update(dt);
		}
	}
}

// -- draw functions -- //

//void DrawDebug(Spaceship ship, Core::Graphics& graphics){
//	strcat("Ship xPos: ", ship.position.x);
//	ship;graphics;
//	graphics.DrawString(10, 10, "");
//}

// -- game functions -- //

bool Update(float dt){
	dt;
	UpdateShipBehavior();
	UpdateSelectedWeapon();
	ship.prev_dt = dt;
	UpdateShipPosition(ship, dt);
	shipUpdateFn(ship, SCREEN_WIDTH, SCREEN_HEIGHT, dt);
	UpdateBullets(dt);

	asteroid.Update(dt);

	for (int i = 0; i <= MAX_BG_INDEX; i++){
		bgObjects[i].Update(dt);
	}

	wasMousePressed = Input::IsPressed(Input::BUTTON_LEFT);
	return (Input::IsPressed(Input::KEY_ESCAPE));
}

void Draw(Core::Graphics& graphics){
	graphics.SetColor(defaultColor);

	graphics.DrawString(250, 30, "Cycle ship collision modes with [F1, F2, F3]");
	graphics.DrawString(260, 50, updateBehaviorTitle);
	graphics.DrawString(250, 70, "Cycle weaponse with [1, 2, 3]");
	graphics.DrawString(260, 90, currentWeaponTitle);

	Turret gun = ship.GetTurret();
	if (gun.isReloading){
		graphics.DrawString(260, 110, "[Reloading:");
		Debug::DrawValue(graphics, 350, 110, Debug::Debug_RoundValue(gun.reloadPercent * 100));
		graphics.DrawString(400, 110, "%]");
	}
	else{
		graphics.DrawString(260, 110, "AMMO:");
		Debug::DrawValue(graphics, 300, 110, float(gun.GetMagazineAmmo()));
		graphics.DrawString(320, 110, "/");
		Debug::DrawValue(graphics, 335, 110, float(gun.GetTotalAmmo()));
	}

	ship.Draw(graphics);
	Obstacles::DrawWalls(graphics);
	asteroid.Draw(graphics);

	for (int i = 0; i <= MAX_BULLET_INDEX; i++){
		if (bullets[i].isVisible){
			bullets[i].Draw(graphics);
		}
	}

	for (int i = 0; i <= MAX_BG_INDEX; i++){
		bgObjects[i].Draw(graphics);
	}
}

void SetupBgObjects(){
	bgObjects[0] = HierarchialTransform(0.8f, 50.0f, 3, Vector2(100, 100));;
	bgObjects[1] = HierarchialTransform(0.8f, 50.0f, 3, Vector2(700, 100));;
	bgObjects[2] = HierarchialTransform(0.8f, 50.0f, 3, Vector2(700, 500));;
	bgObjects[3] = HierarchialTransform(0.8f, 50.0f, 3, Vector2(100, 500));;
}

int main()
{
	asteroid.SetVelocity(100.2f, 100.2f);
	ship.SetPosition(Vector2(400, 400), Vector2(float(Input::GetMouseX()), float(Input::GetMouseY())), 0.0f);

	shipUpdateFn = Update_ScreenWrap;
	updateBehaviorTitle = "CURRENT MODE: Screen Wrap";
	currentWeaponTitle = "CURRENT WEAPON: Single Shot";

	SetupBgObjects();

	Core::Init( "Game Demo", SCREEN_WIDTH, SCREEN_HEIGHT);
	Core::RegisterUpdateFn(Update);
	Core::RegisterDrawFn(Draw);
	Core::GameLoop();
}

