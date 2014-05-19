#include "Core.h"

using Core::Input;

namespace Utils{
	const char TOGGLE_KEY = 'Q';

	class Controls{
		static bool isDisplayed;
		static bool wasToggled;
	public:
		static void ToggleVisibility(){
			isDisplayed = !isDisplayed;
		}
		static void Draw(int x, int y, Core::Graphics& graphics){
			if (isDisplayed){
				graphics.DrawString(x, y, "SPACESHIP CONTROLS");
				graphics.DrawString(x, y + 20, "W = Forward, S = Backward");
				graphics.DrawString(x, y + 40, "A = Rotate Left, D = Rotate Right");
				graphics.DrawString(x, y + 60, "Press left mouse button to fire missiles.");
			}
			else graphics.DrawString(600, 30, "Press 'Q' for controls.");
		}
		static void Update(){
			if (Input::IsPressed(TOGGLE_KEY) && !wasToggled){
				ToggleVisibility();
			}

			wasToggled = Input::IsPressed(TOGGLE_KEY);
		}
	};
}