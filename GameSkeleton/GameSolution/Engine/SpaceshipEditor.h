#ifndef EDITOR_H
#define EDITOR_H

#include "Vector2.h"
#include "Spaceship.h"
#include "Core.h"
#include "Button.h"
#include <vector>

using Core::Input;
using std::vector;

class SpaceshipEditor{
private:
	Button saveButton, clearButton, exitButton;
	bool hasSpaceship, isClicking, wasClicking, exited;
	bool isDrawingLine;
	Vector2 mousePos;
	vector<Vector2*> lines;
	GameObject* obj;
	Vector2* lineStart;
	Vector2 currentMousePos;
	Vector2 origin;
	int minX, maxX, minY, maxY;

	void AddPoint(Vector2* point){
		lines.push_back(point);
	}
	void RemovePoint(int index){
		vector<Vector2*>::iterator removedEff = lines.begin() + index;
		lines.erase(removedEff);
	}
	void RemovePoint(Vector2* point){
		bool found = false;

		unsigned int index = 0;
		vector<Vector2*>::iterator iter = lines.begin(); 
		while(iter != lines.end() && !found && (index < lines.size())){
			if (point == *iter){
				iter = lines.erase(iter);
				found = true;
			}
			else iter = lines.begin() + index;

			index++;
		}
	}
	void UpdateMinMaxPositions(int mouseX, int mouseY){
		if (mouseX < minX){
			minX = int(mouseX - origin.x);
		}
		else if (mouseX > maxX){
			maxX = int(mouseX - origin.x);
		}
		if (mouseY < minY){
			minY = int(mouseY - origin.y);
		}
		else if (mouseY > maxY){
			maxY = int(mouseY - origin.y);
		}
	}
public:
	SpaceshipEditor(int screenWidth, int screenHeight){
		origin = Vector2(float(screenWidth / 2), float(screenHeight / 2));

		lineStart = new Vector2();

		int buttonX = screenWidth - 60;

		saveButton = Button(buttonX, 20, "Save", 10);
		clearButton = Button(buttonX, 60, "Clear", 10);
		exitButton = Button(buttonX, 100, "Exit", 10);

		minX = 0;
		maxX = 0;
		minY = 0;
		maxY = 0;
	}

	void Update(){
		isDrawingLine = false;
		isClicking = Input::IsPressed(Input::BUTTON_LEFT);

		if (!wasClicking && saveButton.IsPressed()){
			int index = 0;

			obj->SetNumberLines(lines.size());
			for(vector<Vector2*>::iterator iter = lines.begin(); iter != lines.end(); iter+=2){
				Vector2* startPtr = *iter;
				Vector2* endPtr = *(iter+1);
				Vector2 start = *startPtr;
				Vector2 end = *endPtr;

				Vector2 normalizedStart = Vector2((start.x - origin.x), (start.y - origin.y));
				Vector2 normalizedEnd = Vector2((end.x - origin.x), (end.y - origin.y));

				obj->SetLine(index, normalizedStart, normalizedEnd);

				index+=2;
			}	

			hasSpaceship = true;
		}
		else if (!wasClicking && clearButton.IsPressed()){
			while(lines.size() > 0){
				RemovePoint(0);
			}
		}
		else if (!wasClicking && exitButton.IsPressed()){
			exited = true;
		}

		int mouseX = Input::GetMouseX();
		int mouseY = Input::GetMouseY();


		if (isClicking){
			isDrawingLine = true;

			if (!wasClicking){
				delete lineStart;
				lineStart = new Vector2(float(mouseX), float(mouseY));

				UpdateMinMaxPositions(mouseX, mouseY);
			}
		}
		else if (!isClicking && wasClicking){
			Vector2* lineEnd = new Vector2(float(mouseX), float(mouseY));
			Vector2 startDeref = *lineStart;

			Vector2* start = new Vector2(startDeref.x, startDeref.y);
			AddPoint(start);
			AddPoint(lineEnd);

			UpdateMinMaxPositions(mouseX, mouseY);
		}

		currentMousePos = Vector2(float(mouseX), float(mouseY));
		wasClicking = isClicking;
	}
	void Draw(Core::Graphics& graphics){
		saveButton.Draw(graphics);
		clearButton.Draw(graphics);
		exitButton.Draw(graphics);

		graphics.DrawLine(origin.x - 2, origin.y, origin.x + 3, origin.y);
		graphics.DrawLine(origin.x, origin.y - 2, origin.x, origin.y + 3);
		graphics.DrawString(int(origin.x + 3), int(origin.y - 3), "(0, 0)");

		graphics.DrawString(20, 20, "WIDTH:");
		Debug::DrawValue(graphics, 65, 20, (maxX - minX));
		graphics.DrawString(20, 40, "HEIGHT:");
		Debug::DrawValue(graphics, 72, 40, (maxY - minY));

		if(isDrawingLine){
			Vector2 start = *lineStart;
			graphics.DrawLine(start.x, start.y, currentMousePos.x, currentMousePos.y);
		}

		int posX = int(currentMousePos.x - origin.x);
		int posY = int(currentMousePos.y - origin.y);

		int firstNumShift = (posX > -100) ? 0 : 7;
		int secondNumShift = (posY > -100) ? 0 : 7;

		int startX = int(currentMousePos.x + 2);
		int startY = int(currentMousePos.y);
		graphics.DrawString(startX, startY, "(");
		Debug::DrawValue(graphics, startX + 5, startY, posX);
		graphics.DrawString(startX + 25 + firstNumShift, startY, ",");
		Debug::DrawValue(graphics, startX + 30 + firstNumShift, startY, posY);
		graphics.DrawString((startX + 50 + firstNumShift + secondNumShift), startY, ")");

		for(vector<Vector2*>::iterator iter = lines.begin(); iter != lines.end(); iter+=2){
			Vector2* start = *iter;
			Vector2* end = *(iter+1);
			Vector2 startLine = *start;
			Vector2 endLine = *end;

			graphics.DrawLine(startLine.x, startLine.y, endLine.x, endLine.y);
		}
	}

	void Reset(){
		exited = false;
		hasSpaceship = false;
		isClicking = false;
		wasClicking = false;
	}

	bool IsExited(){
		return exited;
	}
	bool IsSpaceshipReady(){
		return hasSpaceship;
	}

	void SaveObject(){

	}

	//GameObject* GetSpaceship(){
	//	return obj;
	//}
};

#endif