#ifndef CONTROLS_H
#define CONTROLS_H

#include "Core.h"
using Core::Input;

namespace Controls{
	bool Update(){
		return !Input::IsPressed('B');
	}
	void Draw(Core::Graphics& graphics){
		int xPos = 200;
		int yPos = 80;

		graphics.DrawString(xPos, yPos, "A, D - Left/Right");
		graphics.DrawString(xPos, yPos + 20, "W - Jump");
		graphics.DrawString(xPos, yPos + 40, "Space - Use Item/Weapon");
		graphics.DrawString(xPos, yPos + 80, "* Reach item box at end of level to win.");
		graphics.DrawString(xPos, yPos + 100, "* Avoid all enemies or kill them with weapons (fireballs).");

		graphics.DrawString(xPos, yPos + 140, "PRESS 'B' TO RETURN TO MAIN MENU");
	}
}

#endif