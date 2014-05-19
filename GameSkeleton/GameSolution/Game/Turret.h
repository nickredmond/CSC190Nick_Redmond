#ifndef TURRET_H
#define TURRET_H

#include "MoveablleObject.h"
#include "Bullet.h"
#include "Debug.h"
#include <iostream>
using std::strcmp;

class Turret : public MoveableObject{
private:
	int magazineAmmo, totalAmmo, magazineSize, reloadTime, _length;
	Matrix3 transform;
	Bullet result[3];
public:
	bool isReloading;
	int reloadPercent;
	char* _name;

	Turret(){}

	Turret(int length, char* name, int magAmmo, int totAmmo, int magSize, int reload){
		_length = length;
		_name = name;
		magazineAmmo = magAmmo;
		totalAmmo = totAmmo;
		magazineSize = magSize;
		reloadTime = reload;

		angle = 0.0f;

		numLines = 2;
		lines = new Vector2[numLines];

		lines[0].x = 0;
		lines[0].y = 0;
		lines[1].x = lines[0].x + _length;
		lines[1].y = position.y;
	}

	int GetMagazineAmmo(){
		return magazineAmmo;
	}
	int GetTotalAmmo(){
		return totalAmmo;
	}

	int GetBulletsPerShot(){
		int result = 0;

		if (strcmp(_name, "Triple Shot") == 0){
			result = 3;
		}
		else result = 1;

		return result;
	}

	Bullet* Fire(){
		if (magazineAmmo > 0){
			magazineAmmo--;
			
			if (strcmp(_name, "Single Shot") == 0){
				Vector2 vel = Matrix3::Rotation(angle) * Vector2(BULLET_VELOCITY, 0);
				result[0] = Bullet(vel, Vector2(transform.data[2], transform.data[5]));
			}
			else if (strcmp(_name, "Triple Shot") == 0){
				Vector2 vel1 = Matrix3::Rotation(angle) * Vector2(BULLET_VELOCITY, 0);
				Vector2 vel2 = Matrix3::Rotation(angle + 0.07f) * Vector2(BULLET_VELOCITY, 0);
				Vector2 vel3 = Matrix3::Rotation(angle - 0.07f) * Vector2(BULLET_VELOCITY, 0);

				result[0] = Bullet(vel1, Vector2(transform.data[2], transform.data[5]));
				result[1] = Bullet(vel2, Vector2(transform.data[2], transform.data[5]));
				result[2] = Bullet(vel3, Vector2(transform.data[2], transform.data[5]));
			}
			else if (strcmp(_name, "Laser") == 0){
				Vector2 vel = Matrix3::Rotation(angle) * Vector2(BULLET_VELOCITY, 0);
				result[0] = Bullet(vel, Vector2(transform.data[2], transform.data[5]), true);
				result[0].angle = angle;
			}
		}
		else isReloading = true;

		return result;
	}

	void Update(Vector2 shipPos, Vector2 mousePos, float dt){
		dt;
		Vector2 mouseMinusShip = mousePos - shipPos;
		Vector2 normalized = mouseMinusShip.normalized();
		Vector2 perpCCW = normalized.perpCCW();

		angle = float(atan2(double(normalized.y), double(normalized.x)));

		transform = Matrix3(normalized, perpCCW).Translate(shipPos.x, shipPos.y);// * Matrix3::Translation(shipPos);
	}
	void Draw(Core::Graphics& graphics){
		//Vector2 translation = Vector2(position.x, position.y); //(velocity * prev_dt);
		//Matrix3 transform = Matrix3::Translation(translation) * Matrix3::Rotation(angle);
		DrawObj(graphics, transform);
	}
};

#endif