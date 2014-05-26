#ifndef DEBUG_H
#define DEBUG_H

#include "Core.h"
#include "Matrix3.h"

#include <sstream>

using std::stringstream;
using std::ceil;
using std::floor;

namespace Debug{
	float Debug_RoundValue(float val){
		int normalized = (int)(val * 1000.0f);
		return normalized / 1000.0f;
	}

	void DrawValue( Core::Graphics& graphics, int x, int y, float num ) {
		stringstream ss;
		ss << num;
		graphics.DrawString( x, y, ss.str().c_str());
	}

	void DrawValue( Core::Graphics& graphics, int x, int y, int num ) {
		stringstream ss;
		ss << num;
		graphics.DrawString( x, y, ss.str().c_str());
	}

	void DrawMatrix(int xPos, int yPos, Core::Graphics& graphics, Matrix3 matrix){
		int perRowPixels = 15;
		int perColPixels = 50;

		for (int i = 0; i < sizeof(matrix.data) / sizeof(*matrix.data); i++){
			int rowNumber = i / 3;
			int colNumber = 0;

			if (i == 1 || i == 4 || i == 7){
				colNumber = 1;
			}
			else if (i == 2 || i == 5 || i == 8){
				colNumber = 2;
			}

			float roundedVal = Debug_RoundValue(matrix.data[i]);
			
			int x = xPos + (colNumber * perColPixels);
			int y = yPos + (rowNumber * perRowPixels);

			DrawValue(graphics, x, y, roundedVal);
		}
	}
}

#endif