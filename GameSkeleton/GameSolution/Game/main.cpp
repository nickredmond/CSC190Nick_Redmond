#include "Asteroid.h"
#include "HierarchialTransform.h"
#include "Utils.h"
#include "Wall.h"
#include "UpdateFunctions.h"
#include "ExplosionEffect.h"
#include "BubbleEffect.h"
#include "ParticleManager.h"
#include "StarryNight.h"

using Core::Input;
using namespace Obstacles;
using namespace Utils;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

Asteroid asteroid(Vector2(50, 30));

//ExplosionEffect e= ExplosionEffect(0.1f, 0.15f, ColorChangeType::EXPLOSION, 
//		Vector2(300, 300), 2.0f, 12.0f, 500);

ParticleManager manager;
StarryNight bgStars(SCREEN_WIDTH, SCREEN_HEIGHT, 100, 0.5f);

Core::RGB defaultColor = RGB(255, 255, 255);
Spaceship ship(20, 20);

bool Update(float dt){
	dt;
	UpdateShipBehavior();
	UpdateSelectedWeapon(ship);
	ship.prev_dt = dt;
	UpdateShipPosition(ship, dt);
	bool isCollision = shipUpdateFn(ship, SCREEN_WIDTH, SCREEN_HEIGHT, dt);

	if (isCollision){
		ParticleEffect* explosion = new ExplosionEffect(0.15f, 0.1f, ColorChangeType::FIRE,
			ship.GetPosition(), 1.0f, 10.0f, 215);
		manager.AddEffect(explosion);
	}

	UpdateBullets(dt);
	asteroid.Update(dt);

	Utils::BackgroundObjects::Update();
	if (Utils::isVisible){
		for (int i = 0; i <= MAX_BG_INDEX; i++){
			bgObjects[i].Update(dt);
		}
	}

	manager.Update(Input::IsPressed('W'), ship.angle, ship.GetPosition(), dt);
	bgStars.Update(dt);

	Utils::Controls::Update();

	wasMousePressed = Input::IsPressed(Input::BUTTON_LEFT);
	return (Input::IsPressed(Input::KEY_ESCAPE));
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

	ship.Draw(graphics);
	Obstacles::DrawWalls(graphics);
	asteroid.Draw(graphics);

	Utils::Controls::Draw(300, 230, graphics);

	for (int i = 0; i <= MAX_BULLET_INDEX; i++){
		if (bullets[i].isVisible){
			bullets[i].Draw(graphics);
		}
	}

	if (Utils::isVisible){
		for (int i = 0; i <= MAX_BG_INDEX; i++){
			bgObjects[i].Draw(graphics);
		}
	}
	else Utils::BackgroundObjects::Draw(graphics);

	manager.Draw(graphics);
	bgStars.Draw(graphics);
}

int main()
{
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

