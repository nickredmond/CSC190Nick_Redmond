#include "Asteroid.h"
#include "HierarchialTransform.h"
#include "Utils.h"
#include "Wall.h"
#include "UpdateFunctions.h"
#include "ExplosionEffect.h"
#include "BubbleEffect.h"
#include "ParticleManager.h"
#include "StarryNight.h"

#include "AutoProfile.h"

using Core::Input;
using namespace Obstacles;
using namespace Utils;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

Asteroid asteroid(Vector2(50, 30));
ParticleManager manager;
StarryNight bgStars(SCREEN_WIDTH, SCREEN_HEIGHT, 100, 0.5f);

Core::RGB defaultColor = RGB(255, 255, 255);
Spaceship ship(20, 20);

Timer time = Timer(0.5f);

bool Update(float dt){
	{
		PROFILE("SpaceshipUpdate");
		UpdateShipBehavior();
		UpdateSelectedWeapon(ship);
		ship.prev_dt = dt;
		UpdateShipPosition(ship, dt);
	}

	{
		PROFILE("CollisionUpdate");
		bool isCollision = shipUpdateFn(ship, SCREEN_WIDTH, SCREEN_HEIGHT, dt);

		if (isCollision){
			ParticleEffect* explosion = new ExplosionEffect(0.15f, 0.1f, ColorChangeType::FIRE,
				ship.GetPosition(), 1.0f, 10.0f, 215);
			manager.AddEffect(explosion);
		}
	}

	{
		PROFILE("ProjectilesUpdate");
		UpdateBullets(dt);
	}

	{
		PROFILE("BackgroundUpdate");

		Utils::BackgroundObjects::Update();
		if (Utils::isVisible){
			for (int i = 0; i <= MAX_BG_INDEX; i++){
				bgObjects[i].Update(dt);
			}
		}

		bgStars.Update(dt);
	}

	{
		PROFILE("ParticlesUpdate");
		manager.Update(Input::IsPressed('W'), ship.angle, ship.GetPosition(), dt);
	}

	asteroid.Update(dt);
	Utils::Controls::Update();

	time.Update(dt, true);

	wasMousePressed = Input::IsPressed(Input::BUTTON_LEFT);
	bool requestedExit = (Input::IsPressed(Input::KEY_ESCAPE));

	if (requestedExit){
		profiler.WriteToFile();
	}

	return requestedExit;
}

void Draw(Core::Graphics& graphics){
	graphics.SetColor(defaultColor);

	graphics.DrawString(200, 30, "Cycle ship collision modes with [F1, F2, F3]");
	graphics.DrawString(210, 50, updateBehaviorTitle);
	graphics.DrawString(200, 70, "Cycle weaponse with [1, 2, 3]");
	graphics.DrawString(210, 90, currentWeaponTitle);

	Turret gun = ship.GetTurret();
	if (gun.isReloading){
		graphics.DrawString(210, 110, "[Reloading:");
		Debug::DrawValue(graphics, 300, 110, Debug::Debug_RoundValue(gun.reloadPercent * 100));
		graphics.DrawString(350, 110, "%]");
	}
	else{
		graphics.DrawString(210, 110, "AMMO:");
		Debug::DrawValue(graphics, 250, 110, float(gun.GetMagazineAmmo()));
		graphics.DrawString(270, 110, "/");
		Debug::DrawValue(graphics, 285, 110, float(gun.GetTotalAmmo()));
	}

	{
		PROFILE("SpaceshipDraw");
		ship.Draw(graphics);
	}

	Obstacles::DrawWalls(graphics);
	asteroid.Draw(graphics);

	Utils::Controls::Draw(300, 230, graphics);

	time.Draw(graphics);

	{
		PROFILE("ProjectilesDraw");
		for (int i = 0; i <= MAX_BULLET_INDEX; i++){
			if (bullets[i].isVisible){
				bullets[i].Draw(graphics);
			}
		}
	}

	{
		PROFILE("BackgroundDraw");

		if (Utils::isVisible){
			for (int i = 0; i <= MAX_BG_INDEX; i++){
				bgObjects[i].Draw(graphics);
			}
		}
		else Utils::BackgroundObjects::Draw(graphics);

		bgStars.Draw(graphics);
	}

	{
		PROFILE("ParticlesDraw");
		manager.Draw(graphics);
	}
}

int main()
{
	profiler.AddCategory("SpaceshipUpdate");
	profiler.AddCategory("CollisionUpdate");
	profiler.AddCategory("ParticlesUpdate");
	profiler.AddCategory("ProjectilesUpdate");
	profiler.AddCategory("BackgroundUpdate");

	profiler.AddCategory("SpaceshipDraw");
	profiler.AddCategory("ProjectilesDraw");
	profiler.AddCategory("BackgroundDraw");
	profiler.AddCategory("ParticlesDraw");

	asteroid.SetVelocity(100.2f, 100.2f);
	ship.SetPosition(Vector2(400, 400), Vector2(float(Input::GetMouseX()), float(Input::GetMouseY())), 0.0f);

	BubbleEffect* bubbles = new BubbleEffect(0.08f, 0.05f, ColorChangeType::FIRE, 
		Vector2(220, 220), 0.01f, 0.1f, 6.0f, 18.0f, 1.0f, 5.0f, 150);
	manager.AddEffect(bubbles);

	shipUpdateFn = Update_ScreenWrap;
	updateBehaviorTitle = "CURRENT MODE: Screen Wrap";
	currentWeaponTitle = "CURRENT WEAPON: Single Shot";

	Utils::isDisplayed = false;
	Utils::wasToggled = false;
	Utils::isVisible = false;
	Utils::wasBgToggled = false;

	SetupBgObjects();

	Core::Init( "Game Demo", SCREEN_WIDTH, SCREEN_HEIGHT);
	Core::RegisterUpdateFn(Update);
	Core::RegisterDrawFn(Draw);
	Core::GameLoop();
}

