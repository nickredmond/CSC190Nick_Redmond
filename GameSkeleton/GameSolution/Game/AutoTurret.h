#ifndef AUTO_TURRET_H
#define AUTO_TURRET_H

#include "Enemy.h"

class AutoTurret : public Turret {
private:
	class TurretGun : public MoveableObject{
	private:
		void SetPosition(Vector2 pos){
			pos;
			position.x = 0;
			position.y = 0;

			lines[0].x = position.x - 5;
			lines[0].y = position.y - 5;
			lines[1].x = position.x + 10;
			lines[1].y = position.y;
			lines[2].x = position.x + 10;
			lines[2].y = position.y;
			lines[3] = position.x - 5;
			lines[3].y = position.y + 5;
			lines[4] = position.x - 5;
			lines[4].y = position.y + 5;
			lines[5].x = position.x - 5;
			lines[5].y = position.y - 5;
		}
	public:
		TurretGun(){}
		TurretGun(Vector2 pos){
			numLines = 6;
			lines = new Vector2[numLines];

			SetPosition(pos);
		}
		bool IsAlive(){return true;}
	};

	typedef Turret super;
	float fireRate, timeSinceFired;
	TurretGun gun;

	void SetPosition(Vector2 pos){
		position.x = pos.x;
		position.y = pos.y;
		
		lines[0].x = position.x - 10;
		lines[0].y = position.y - 10;
		lines[1].x = position.x + 10;
		lines[1].y = position.y - 10;
		lines[2].x = position.x + 10;
		lines[2].y = position.y - 10;
		lines[3].x = position.x + 10;
		lines[3].y = position.y + 10;
		lines[4].x = position.x + 10;
		lines[4].y = position.y + 10;
		lines[5].x = position.x - 10;
		lines[5].y = position.y + 10;
		lines[6].x = position.x - 10;
		lines[6].y = position.y + 10;
		lines[7].x = position.x - 10;
		lines[7].y = position.y - 10;
	}
public:
	AutoTurret(){

	}

	AutoTurret(char* name, Vector2 pos, float _fireRate = 1.0f){
		_name = name;
		fireRate = _fireRate;

		timeSinceFired = 0.0f;
		numLines = 14;
		lines = new Vector2[numLines];

		gun = TurretGun(pos);
		SetPosition(pos);
	}

	bool CanFire(){
		return (timeSinceFired >= fireRate);
	}

	void Update(Vector2 shipPos, float dt){
		timeSinceFired += dt;

		Vector2 shipMinusPos = shipPos - position;
		Vector2 normalized = shipMinusPos.normalized();
		Vector2 perpCCW = normalized.perpCCW();

		angle = float(atan2(double(normalized.y), double(normalized.x)));
		transform = Matrix3(normalized, perpCCW).Translate(position);
	}
	void Draw(Core::Graphics& graphics){
		DrawObj(graphics, Matrix3());
		gun.DrawObj(graphics, transform);
	}

	Bullet* Fire(){
		timeSinceFired = 0.0f;
		return GetBullets();
	}

	bool IsAlive(){
		return true;
	}
};

#endif