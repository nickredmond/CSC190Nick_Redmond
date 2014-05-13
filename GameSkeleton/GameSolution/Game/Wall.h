#include "Vector2.h"

namespace Obstacles
{
	const float BOUNCE_COEFF = 0.7f;
	const float NUM_WALLS = 4;

	class Wall{
	public:
		Vector2 v1, v2;

		Wall(Vector2 _v1, Vector2 _v2){
			v1 = _v1;
			v2 = _v2;
		}
		Wall(){

		}

		Vector2 normalizedPerp(){
			Vector2 p = v2 - v1;
			p = p.normalized();
			return p.perpCCW();
		}
	};

	Wall* GetWalls(){
		Wall walls[4];

		walls[0] = Wall(Vector2(500, 0), Vector2(800, 200));
		walls[1] = Wall(Vector2(0, 400), Vector2(500, 0));
		walls[2] = Wall(Vector2(300, 600), Vector2(0, 400));
		walls[3] = Wall(Vector2(800, 200), Vector2(300, 600));

		Wall* wallPtr = walls;
		return wallPtr;
	}

	void DrawWalls(Core::Graphics& graphics){
		Wall* walls = GetWalls();

		//for (int i = 0; i < sizeof(walls) / sizeof(Wall); i++){
		//	Wall nextWall = walls[i];
		//	graphics.DrawLine(nextWall.v1.x, nextWall.v1.y, nextWall.v2.x, nextWall.v2.y);
		//}

		Wall wall1 = walls[0];
		Wall wall2 = walls[1];
		Wall wall3 = walls[2];
		Wall wall4 = walls[3];
		
		graphics.DrawLine(wall1.v1.x, wall1.v1.y, wall1.v2.x, wall1.v2.y);
		graphics.DrawLine(wall2.v1.x, wall2.v1.y, wall2.v2.x, wall2.v2.y);
		graphics.DrawLine(wall3.v1.x, wall3.v1.y, wall3.v2.x, wall3.v2.y);
		graphics.DrawLine(wall4.v1.x, wall4.v1.y, wall4.v2.x, wall4.v2.y);
	}

	bool IsCollision(Spaceship& ship, Wall& wall, float dt){
		Vector2 n = wall.normalizedPerp();

		Vector2 q1 = ship.GetPosition() - wall.v1;
		float d1 = q1.dotProduct(n);

		Vector2 afterPos = ship.GetPosition() + (ship.GetVelocity() * dt);
		Vector2 q2 = afterPos - wall.v1;
		float d2 = q2.dotProduct(n);

		return (d1 > 0 && d2 < 0);
	}

	void RespondToCollisions(Spaceship& ship, Wall& wall){
		Vector2 n = wall.normalizedPerp();
		Vector2 vel = ship.GetVelocity();
		Vector2 newVelocity = vel - ((1.0f + BOUNCE_COEFF) * vel.dotProduct(n) * n);

		ship.SetVelocity(newVelocity.x, newVelocity.y);
	}
}