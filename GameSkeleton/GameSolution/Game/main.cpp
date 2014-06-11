#include "Wall.h"
#include "ExplosionEffect.h"
#include "BubbleEffect.h"
#include "Logger.h"
#include "AutoProfile.h"
#include "SpaceshipEditor.h"

#include "Player.h"
#include "UpdateFunctions.h"
#include "Collisions.h"
#include "Level.h"
#include "MainMenu.h"
#include "Controls.h"

using Core::Input;
using namespace Obstacles;
using namespace Utils;

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

const int NO_SELECTION = -1;
const int START_GAME = 0;
const int CONTROLS = 1;

Core::RGB defaultColor = RGB(255, 255, 255);

Timer time = Timer(0.5f);
SpaceshipEditor editor = SpaceshipEditor(SCREEN_WIDTH, SCREEN_HEIGHT);
MainMenu menu(Vector2(float(SCREEN_WIDTH / 2), float(SCREEN_HEIGHT / 2)));

ParticleManager* manager = new ParticleManager();
Level lvl;
int currentLevelIndex = 0;
int numberLevels = 1;
Level* levels;
bool isMainMenu = false;
bool isControls = false;

bool isEditingSpaceship;

void ResetLevel(){
	lvl = GetDefaultLevel(SCREEN_WIDTH, manager);
	lvl.screenWidth = SCREEN_WIDTH;
	lvl.screenHeight = SCREEN_HEIGHT;
}

void UpdateGameState(){
	if (isMainMenu){
		int menuChoice = menu.GetSelectedItem();

		if (menuChoice == START_GAME){
			ResetLevel();
			menu.Reset();
			isMainMenu = false;
		}
		else if (menuChoice == CONTROLS){
			isControls = true;
			ResetLevel();
			menu.Reset();
			isMainMenu = false;
		}	
	}
}

bool Update(float dt){dt;
	if (isEditingSpaceship){
		editor.Update();

		if (editor.IsExited() || editor.IsSpaceshipReady()){
			isEditingSpaceship = false;

			if (editor.IsSpaceshipReady()){
		//		ship = editor.GetSpaceship();
			}

			editor.Reset();
		}
	}
	else{
		if(!isMainMenu && !isControls){
			lvl.Update(dt);
			manager->Update(false, 0.0f, Vector2(), dt);

			if (lvl.IsWin() || lvl.IsGameOver()){
				isMainMenu = true;
			}
		}
		else if (isControls){
			isControls = Controls::Update();
			if (!isControls){
				isMainMenu = true;
			}
		}
		else{
			menu.Update(dt);
			UpdateGameState();
		}
	}

	if (Input::IsPressed('O')){
		isEditingSpaceship = true;
	}

	bool requestedExit = (Input::IsPressed(Input::KEY_ESCAPE));
	if (requestedExit){
		profiler.WriteToFile();
		END_LOG
	}

	return requestedExit;
}

void Draw(Core::Graphics& graphics){
	if (isEditingSpaceship){
		editor.Draw(graphics);
	}
	else{
		graphics.SetColor(defaultColor);

		if(!isMainMenu && !isControls){
			{
				PROFILE("LevelDraw");
				lvl.Draw(graphics);
			}
			{
				PROFILE("ParticlesDraw");
				manager->Draw(graphics);
			}
		}
		else if (isControls){
			Controls::Draw(graphics);
		}
		else{
			menu.Draw(graphics);
		}
	}
}

int main()
{
	bool test = false;
	ASSERT(test, "Test :: If this line gets written, then the assertion system works correctly.");

	profiler.AddCategory("LevelUpdate");
	profiler.AddCategory("LevelDraw");
	profiler.AddCategory("ParticlesUpdate");
	profiler.AddCategory("ParticlesDraw");

	isMainMenu = true;
	LOG(Info, "Game initialized");

	Core::Init( "Game Demo", SCREEN_WIDTH, SCREEN_HEIGHT);
	Core::RegisterUpdateFn(Update);
	Core::RegisterDrawFn(Draw);
	Core::GameLoop();
}

