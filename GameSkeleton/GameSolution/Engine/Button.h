#ifndef BUTTON_H
#define BUTTON_H

#include "Core.h"
#include <string>
using Core::Input;
using std::string;

const int PER_CHAR_WIDTH = 7;
const int LINE_HEIGHT = 10;

class Button{
private:
	int xPos, yPos, margin;
	const char* text;
public:
	Button(){
		xPos = 0;
		yPos = 0;
		text = "Not Implemented";
		margin = 0;
	}
	Button(int x, int y, const char* message, int _margin = 2){
		xPos = x;
		yPos = y;
		margin = _margin;
		text = message;
	}

	bool IsPressed(){
		int mouseX = Input::GetMouseX();
		int mouseY = Input::GetMouseY();

		string textStr = text;
		int right = xPos + (textStr.length() * PER_CHAR_WIDTH);
		int bottom = yPos + LINE_HEIGHT;

		bool isInBounds = ((mouseX > xPos - margin) && (mouseX < right + margin) && (mouseY > yPos - margin) && (mouseY < bottom + margin));
		return (isInBounds && Input::IsPressed(Input::BUTTON_LEFT));
	}
	void Draw(Core::Graphics& graphics){
		string textStr = text;
		int right = xPos + (textStr.length() * PER_CHAR_WIDTH);
		int bottom = yPos + LINE_HEIGHT;

		Core::RGB buttonColor = RGB(255, 255, 255);
		graphics.SetColor(buttonColor);

		graphics.DrawLine(float(xPos - margin), float(yPos - margin), float(right + margin), float(yPos - margin));
		graphics.DrawLine(float(right + margin), float(yPos - margin), float(right + margin), float(bottom + margin));
		graphics.DrawLine(float(right + margin), float(bottom + margin), float(xPos - margin), float(bottom + margin));
		graphics.DrawLine(float(xPos - margin), float(bottom + margin), float(xPos - margin), float(yPos - margin));

		graphics.DrawString(xPos, yPos, text);
	}
};

#endif