#ifndef GAME_OBJ_H
#define GAME_OBJ_H

#include "Vector2.h"
#include "Core.h"

class GameObject{
protected:
	Vector2 position;
	float width;
	float height;
public:
	Vector2* lines;
	int numLines;
	float radius;

	virtual bool Update(float dt) = 0;
	virtual void Draw(Core::Graphics& graphics) = 0;
	
	Vector2 GetPosition(){
		return position;
	}
	void SetPosition(Vector2 pos){
		position.x = pos.x;
		position.y = pos.y;
	}

	void SetNumberLines(int _numLines){
		numLines = _numLines;
		delete [] lines;
		lines = new Vector2[numLines];
	}

	void SetLine(int index, Vector2 lineStart, Vector2 lineEnd){
		lines[index].x = lineStart.x;
		lines[index].y = lineStart.y;
		
		lines[index+1].x = lineEnd.x;
		lines[index+1].y = lineEnd.y;
	}

	float GetWidth(){
		return width;
	}
	float GetHeight(){
		return height;
	}
};

#endif