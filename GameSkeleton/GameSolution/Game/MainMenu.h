#ifndef MAINMENU_H
#define MAINMENU_H

#include "GameObject.h"
#include "DebugMemory.h"

const int PER_LINE_INCREASE = 25;

class MainMenu : public GameObject {
private:
	int numberMenuItems, currentMenuIndex, selectedItem;
	bool wasKeyPressed;
	char** menuItems;
public:
	MainMenu(Vector2 pos){
		wasKeyPressed = false;
		selectedItem = -1;
		numberMenuItems = 2;
		menuItems = new char*[numberMenuItems];

		menuItems[0] = "START GAME";
		menuItems[1] = "CONTROLS";

		position.x = pos.x;
		position.y = pos.y;
	}
	int GetSelectedItem(){
		return selectedItem;
	}
	void Reset(){
		selectedItem = -1;
	}
	void Destroy(){
		delete menuItems;
	}

	bool Update(float dt){
		dt;
		bool isValidIndex = (currentMenuIndex >= 0 && currentMenuIndex < numberMenuItems);
		ASSERT(isValidIndex, "Index OutOfBounds :: menuIndex is out of acceptable range.")

		if (Core::Input::IsPressed('S')){
			if (!wasKeyPressed){
				currentMenuIndex++;
				if (currentMenuIndex >= numberMenuItems){
					currentMenuIndex = 0;
				}
			}

			wasKeyPressed = true;
		}
		else if (Core::Input::IsPressed('W')){
			if (!wasKeyPressed){
				currentMenuIndex--;
				if(currentMenuIndex < 0){
					currentMenuIndex = numberMenuItems - 1;
				}
			}

			wasKeyPressed = true;
		}
		else wasKeyPressed = false;

		if (Core::Input::IsPressed(' ')){
			selectedItem = currentMenuIndex;
		}

		return true;
	}
	void Draw(Core::Graphics& graphics){
		graphics.DrawString(int(position.x - 25), int(position.y - (2 * PER_LINE_INCREASE)), "TERRIBLE SUPER MARIO 3 RIPOFF");

		for (int i = 0; i < numberMenuItems; i++){
			char* message = menuItems[i];
			int yPos = int(position.y + (i * PER_LINE_INCREASE));
			graphics.DrawString(int(position.x - 10), yPos, message);
		}

		graphics.DrawString(int(position.x - 10), int(position.y + ((numberMenuItems + 1) * PER_LINE_INCREASE)), "*** W/S = Up/Down *** SPACE = Select ***");

		int pointerPos = int(position.y + (currentMenuIndex * PER_LINE_INCREASE));
		graphics.DrawLine(position.x - 30, float(pointerPos - 5), position.x - 20, float(pointerPos + 5));
		graphics.DrawLine(position.x - 30, float(pointerPos + 15), position.x - 20, float(pointerPos + 5));
	}
};

#endif