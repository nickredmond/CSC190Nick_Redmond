#ifndef WALL_H
#define WALL_H

#include "MoveablleObject.h"

namespace Obstacles
{
	float _d1;
	float _d2;

	const float BOUNCE_COEFF = 0.7f;
	const float NUM_WALLS = 4;
	const float PLATFORM_VELOCITY = 50.0f;

	class Wall : public MoveableObject{
	private:
		Vector2 shift;
		Vector2* path;
		int numberPoints;
		bool isLerping;
		float xDiff, yDiff, beta;
		int currentPathLegIndex;

		int GetNextIndex(int index){
			return (index < numberPoints - 1) ? index + 1 : 0;
		}
	public:
		Vector2 v1, v2;

		Wall(Vector2 _v1, Vector2 _v2, bool _isLerping = false){
			v1 = _v1;
			v2 = _v2;

			xDiff = v2.x - v1.x;
			yDiff = v2.y - v1.y;
			currentPathLegIndex = 0;
			beta = 0.0f;

			isLerping = _isLerping;
			numberPoints = 0;
			path = new Vector2[1];
			velocity = Vector2(PLATFORM_VELOCITY, 0.0f);
		}
		Wall(){

		}

		bool IsLerping(){
			return isLerping;
		}
		void Shift(Vector2 _shift){
			shift.x += _shift.x;
			shift.y += _shift.y;
		}
		void AddPathPoint(Vector2 point){
			Vector2* currentPath = path;
			numberPoints++;

			path = new Vector2[numberPoints];

			for (int i = 0; i < numberPoints - 1; i++){
				path[i] = currentPath[i];
			}
			path[numberPoints - 1] = point;

			if (numberPoints == 1){
				v1 = point;
				v2.x = v1.x + xDiff;
				v2.y = v1.y + xDiff;
			}
		}

		Vector2 normalizedPerp(){
			Vector2 p = v2 - v1;
			p = p.normalized();
			return p.perpCCW();
		}		
		
		bool Update(float dt){
			dt;

			if (isLerping){
				float distanceTraveled = velocity.length() * dt;
				float legDistance = distance(path[currentPathLegIndex], path[GetNextIndex(currentPathLegIndex)]);
				float deltaBeta = distanceTraveled / legDistance;

				if (beta + deltaBeta >= 1){
					beta = deltaBeta - (1 - beta);
					currentPathLegIndex = GetNextIndex(currentPathLegIndex);
				}
				else beta += deltaBeta;

				Vector2 start = path[currentPathLegIndex];
				Vector2 end = path[GetNextIndex(currentPathLegIndex)];

				position = LERP(start, end, beta) + shift;
			}

			return true;
		}
		void Draw(Core::Graphics& graphics){
			if (isLerping){
				v1 = position;
				v2.x = v1.x + xDiff;
				v2.y = v1.y + yDiff;
			}

			graphics.DrawLine(v1.x, v1.y, v2.x, v2.y);
		}
	};

//	void Draw(Core::Graphics& graphics){
//		graphics.DrawLine(top.x, top.y, right.x, right.y);
//		graphics.DrawLine(right.x, right.y, bottom.x, bottom.y);
//		graphics.DrawLine(bottom.x, bottom.y, left.x, left.y);
//		graphics.DrawLine(left.x, left.y, top.x, top.y);
//	}
//	void Update(float dt, Vector2 shipPos){
//		if (isLerping){
//			float distanceTraveled = velocity.length() * dt;
//			float legDistance = distance(path[currentPathLegIndex], path[GetNextIndex(currentPathLegIndex)]);
//			float deltaBeta = distanceTraveled / legDistance;
//
//			if (beta + deltaBeta >= 1){
//				beta = deltaBeta - (1 - beta);
//				currentPathLegIndex = GetNextIndex(currentPathLegIndex);
//			}
//			else beta += deltaBeta;
//
//			Vector2 start = path[currentPathLegIndex];
//			Vector2 end = path[GetNextIndex(currentPathLegIndex)];
//
//			Vector2 currentPos = LERP(start, end, beta);
//			SetPosition(currentPos);
//		}
//		else{
//			Vector2 shipMinusPos = shipPos - position;
//			Vector2 newVel = shipMinusPos.normalized() * DEFAULT_ACCEL;
//			velocity.x = newVel.x;
//			velocity.y = newVel.y;
//
//			Vector2 pos = position + velocity;
//			SetPosition(pos);
//		}
//	}
}

#endif