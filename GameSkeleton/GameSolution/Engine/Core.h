#ifndef CORE_H
#define CORE_H

#ifdef CORE_EXPORTS
#define CORE_SHARED __declspec(dllexport)
#else
#define CORE_SHARED __declspec(dllimport)
#endif

#include "window.h"
#include "graphics.h"
#include "Input.h"

namespace Core
{
	class Graphics;

	typedef bool UpdateFn( float dt );
	typedef void DrawFn( Graphics& graphics);

	CORE_SHARED void Init(char* name, unsigned width, unsigned height, unsigned targetFPS = 60 );
	CORE_SHARED void Shutdown();
	CORE_SHARED void RegisterUpdateFn(UpdateFn* _udpate);
	CORE_SHARED void RegisterDrawFn(DrawFn* _draw);
	CORE_SHARED void GameLoop();

	//void DrawValue( Core::Graphics& graphics, int x, int y, float num ) {
	//	stringstream ss;
	//	ss << num;
	//	graphics.DrawString( x, y, ss.str().c_str());
	//}	
}

#endif // CORE_H