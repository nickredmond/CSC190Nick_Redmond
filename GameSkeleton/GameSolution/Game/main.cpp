#include "Spaceship.h"
#include "Wall.h"
#include "Asteroid.h"

using Core::Input;
using namespace Obstacles;

typedef void (*UpdateShip_CheckEnvironment)(Spaceship& ship, int screenWidth, int screenHeight, float dt);
UpdateShip_CheckEnvironment shipUpdateFn;
char* updateBehaviorTitle;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

Spaceship ship(20, 20);
Asteroid asteroid(Vector2(50, 30));

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
	//Wall w = walls[0];

	float xVelInc = 0;
	float yVelInc = 0;

	if (Input::IsPressed(Input::KEY_LEFT)){
		xVelInc = -ACCELERATION;
	}
	else if (Input::IsPressed(Input::KEY_RIGHT)){
		xVelInc = ACCELERATION;
	}
	else{
		if (ship.GetVelocity().x > 0){
			xVelInc = -ACCELERATION / 2;
		}
		else if (ship.GetVelocity().x < 0){
			xVelInc = ACCELERATION / 2;
		}
	}

		if (Input::IsPressed(Input::KEY_UP)){
		yVelInc = -ACCELERATION;
	}
	else if (Input::IsPressed(Input::KEY_DOWN)){
		yVelInc = ACCELERATION;
	}
	else{
		if (ship.GetVelocity().y > 0){
			yVelInc = -ACCELERATION / 2;
		}
		else if (ship.GetVelocity().y < 0){
			yVelInc = ACCELERATION / 2;
		}
	}

	ship.UpdatePosition(xVelInc, yVelInc, dt);
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

	ship.SetPosition(Vector2(newX, newY));
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
	ship.prev_dt = dt;
	UpdateShipPosition(ship, dt);
	shipUpdateFn(ship, SCREEN_WIDTH, SCREEN_HEIGHT, dt);

	asteroid.Update(dt);

	return (Input::IsPressed(Input::KEY_ESCAPE));
}

void Draw(Core::Graphics& graphics){
	graphics.SetColor(RGB(255, 255, 255));
	ship.Draw(graphics);
	Obstacles::DrawWalls(graphics);
	asteroid.Draw(graphics);

	graphics.DrawString(560, 450, "BEHAVIOR MODES:");
	graphics.DrawString(600, 470, "F1 -- Screen Wrap");
	graphics.DrawString(600, 490, "F2 -- Bounce");
	graphics.DrawString(600, 510, "F3 -- Collisions");
	graphics.DrawString(560, 530, updateBehaviorTitle);
}

int main()
{
	asteroid.SetVelocity(100.2f, 100.2f);
	ship.SetPosition(Vector2(400, 400));

	shipUpdateFn = Update_ScreenWrap;
	updateBehaviorTitle = "CURRENT MODE: Screen Wrap";

	Core::Init( "Game Demo", SCREEN_WIDTH, SCREEN_HEIGHT);
	Core::RegisterUpdateFn(Update);
	Core::RegisterDrawFn(Draw);
	Core::GameLoop();
}
