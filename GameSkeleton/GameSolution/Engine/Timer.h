#ifndef TIMER_H
#define TIMER_H

#include "Core.h"
#include "Debug.h"

class Timer{
private:
	LARGE_INTEGER startTime;
	LARGE_INTEGER stopTime;
	LARGE_INTEGER frequency;
	LARGE_INTEGER lastLap;

	float refreshRate, elapsedTime, secsPerFrame, fps;
	int numFrames;
	bool isDrawable;
	int xPos, yPos;

	double ConvertToSecs(LARGE_INTEGER& time){
		return ((double)time.QuadPart / (double)frequency.QuadPart);
	}
public:
	Timer(float _refreshRate = 0.0f, int _xPos = 20, int _yPos = 20){
		QueryPerformanceFrequency(&frequency);
		startTime.QuadPart = 0;
		stopTime.QuadPart = 0;
		QueryPerformanceCounter(&lastLap);

		refreshRate = _refreshRate;
		elapsedTime = 0.0f;
		secsPerFrame = 0.0f;
		fps = 0.0f;
		isDrawable = true;
		xPos = _xPos;
		yPos = _yPos;
	}

	void Start(){
		startTime.QuadPart = 0;
		QueryPerformanceCounter(&startTime);
	}
	float Stop(){
		LARGE_INTEGER time;
		QueryPerformanceCounter(&stopTime);
		
		time.QuadPart = stopTime.QuadPart - startTime.QuadPart;
		stopTime.QuadPart = 0;
		startTime.QuadPart = 0;

		return float(ConvertToSecs(time));
	}
	float Interval(){
		LARGE_INTEGER time;
		LARGE_INTEGER lapTime;
		lapTime.QuadPart = 0;
		QueryPerformanceCounter(&lapTime);

		time.QuadPart = lapTime.QuadPart - lastLap.QuadPart;
		lastLap.QuadPart = lapTime.QuadPart;
		return float(ConvertToSecs(time));
	}

	void Update(float dt, bool drawable){
		isDrawable = drawable;
		elapsedTime += dt;
		numFrames++;

		if (elapsedTime >= refreshRate){
			fps = float(numFrames) / elapsedTime;
			secsPerFrame = 1.0f / fps;

			elapsedTime = 0.0f;
			numFrames = 0;
		}
	}

	void Draw(Core::Graphics& graphics){
		if (isDrawable){
			Core::RGB white = RGB(255, 255, 255);
			graphics.SetColor(white);

			graphics.DrawString(xPos, yPos, "FPS:");
			Debug::DrawValue(graphics, (xPos + 30), yPos, Debug::Debug_RoundValue(fps));

			graphics.DrawString(xPos, (yPos + 20), "Secs. Per Frame:");
			Debug::DrawValue(graphics, (xPos + 120), (yPos + 20), Debug::Debug_RoundValue(secsPerFrame));
		}
	}
};

#endif