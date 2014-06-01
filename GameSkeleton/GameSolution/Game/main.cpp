#include "Asteroid.h"
#include "HierarchialTransform.h"
#include "EnemyManager.h"
#include "Wall.h"
#include "ExplosionEffect.h"
#include "BubbleEffect.h"
#include "StarryNight.h"

#include "AutoProfile.h"

using Core::Input;
using namespace Obstacles;
using namespace Utils;

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

Asteroid asteroid(Vector2(50, 30));
ParticleManager manager;
EnemyManager em;
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

	asteroid.Update(dt, ship.GetPosition());
	Utils::Controls::Update();

	{
		PROFILE("EnemiesUpdate");
		em.Update(ship, dt);
	}

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

	graphics.DrawString(20, 110, "Cycle ship collision modes with [F1, F2, F3]");
	graphics.DrawString(30, 130, updateBehaviorTitle);
	graphics.DrawString(20, 150, "Cycle weaponse with [1, 2, 3]");
	graphics.DrawString(30, 170, currentWeaponTitle);

	Turret gun = ship.GetTurret();
	if (gun.isReloading){
		graphics.DrawString(30, 190, "[Reloading:");
		Debug::DrawValue(graphics, 120, 190, Debug::Debug_RoundValue(gun.reloadPercent * 100));
		graphics.DrawString(170, 190, "%]");
	}
	else{
		graphics.DrawString(30, 190, "AMMO:");
		Debug::DrawValue(graphics, 70, 190, float(gun.GetMagazineAmmo()));
		graphics.DrawString(90, 190, "/");
		Debug::DrawValue(graphics, 105, 190, float(gun.GetTotalAmmo()));
	}

	{
		PROFILE("SpaceshipDraw");
		ship.Draw(graphics);
	}

	Obstacles::DrawWalls(graphics);
	asteroid.Draw(graphics);

	Utils::Controls::Draw(350, 280, graphics);

	time.Draw(graphics);

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
		PROFILE("ProjectilesDraw");
		for (int i = 0; i <= MAX_BULLET_INDEX; i++){
			if (bullets[i].isVisible){
				bullets[i].Draw(graphics);
			}
		}
	}

	{
		PROFILE("ParticlesDraw");
		manager.Draw(graphics);
	}

	{
		PROFILE("EnemiesDraw");
		em.Draw(graphics);
	}
}

int main()
{
	profiler.AddCategory("SpaceshipUpdate");
	profiler.AddCategory("CollisionUpdate");
	profiler.AddCategory("ParticlesUpdate");
	profiler.AddCategory("ProjectilesUpdate");
	profiler.AddCategory("BackgroundUpdate");
	profiler.AddCategory("EnemiesUpdate");

	profiler.AddCategory("SpaceshipDraw");
	profiler.AddCategory("ProjectilesDraw");
	profiler.AddCategory("BackgroundDraw");
	profiler.AddCategory("ParticlesDraw");
	profiler.AddCategory("EnemiesDraw");

	bullets = new Bullet[MAX_BULLET_INDEX + 1];

	for (int i = 0; i < MAX_BULLET_INDEX + 1; i++){
		bullets[i] = Bullet();
	}

	em = EnemyManager(manager, bullets, 10, 0.5f, SCREEN_WIDTH, SCREEN_HEIGHT);

	asteroid.SetVelocity(100.2f, 100.2f);
	ship.SetPosition(Vector2(400, 400), Vector2(float(Input::GetMouseX()), float(Input::GetMouseY())), 0.0f);

	BubbleEffect* bubbles = new BubbleEffect(0.08f, 0.05f, ColorChangeType::FIRE, 
		Vector2(220, 220), 0.01f, 0.1f, 6.0f, 18.0f, 1.0f, 5.0f, 125);
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

