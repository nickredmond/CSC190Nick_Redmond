#include "Core.h"
#include "Matrix3.h"

#include <iostream>

using std::ceil;
using std::floor;

namespace Debug{

	void DrawMatrix(int xPos, int yPos, Core::Graphics& graphics, Matrix3 matrix){
		int perRowPixels = 10;
		int perColPixels = 25;

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

			

			graphics.DrawString(x, y , "");
		}
	}

	float Debug_RoundValue(float val){
		int normalized = (int)(val * 1000.0f);
		return normalized / 1000.0f;
	}
}