#include "Star.h"

class StarryNight{
private:
	int screenWidth, screenHeight, numStars;
	float spawnRate, timeSinceSpawn;
	Star* stars;
public:
	StarryNight(int width, int height, int nrStars, float spawnRt){
		screenWidth = width;
		screenHeight = height;
		numStars = nrStars;
		spawnRate = spawnRt;
		
		timeSinceSpawn = 0.0f;
		stars = new Star[numStars];
	}

	void Update(float dt){
		timeSinceSpawn += dt;

		for (int i = 0; i < numStars; i++){
			if (stars[i].IsAlive()){
				stars[i].Update(dt);
			}
			else if (timeSinceSpawn > spawnRate){
				float x = Utils::randomInRange(0.0f, float(screenWidth));
				float y = Utils::randomInRange(0.0f, float(screenHeight));
				float maxBright = Utils::randomFloat();
				float lifetime = Utils::randomInRange(0.1f, 1.0f);

				stars[i] = Star(Vector2(x, y), maxBright, lifetime);
			}
		}
	}

	void Draw(Core::Graphics& graphics){
		for (int i = 0; i < numStars; i++){
			if (stars[i].IsAlive()){
				stars[i].Draw(graphics);
			}
		}
	}
};