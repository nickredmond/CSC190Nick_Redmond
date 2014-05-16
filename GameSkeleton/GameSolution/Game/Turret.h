#ifndef TURRET_H
#define TURRET_H

#include "MoveablleObject.h"
#include "Bullet.h"

class Turret : public MoveableObject{
private:
	int magazineAmmo, totalAmmo, magazineSize, reloadTime, _length;
	Matrix3 transform;
public:
	bool isReloading;
	int reloadPercent;

	Turret(int length){
		_length = length;

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
	Bullet Fire(){
		Bullet result;

		if (magazineAmmo > 0){
			magazineAmmo--;
			// isReloading... dt... Update()...
		}
	}
	void Update(Vector2 shipPos, Vector2 mousePos){

	}
	void Draw(Core::Graphics& graphics){
		//Vector2 translation = Vector2(position.x, position.y); //(velocity * prev_dt);
		//Matrix3 transform = Matrix3::Translation(translation) * Matrix3::Rotation(angle);

		DrawObj(graphics, transform);
	}
};

#endif