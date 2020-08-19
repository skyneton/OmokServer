#pragma once
#include "GameObject.h"
#include "RadiusCollider.h"
class Bullet :
	public GameObject
{
public:
	RadiusCollider* col;
	float speed, speedRate; //속도, 가속도
	float angle, angleRate; //각도, 각속도
	float damage; //데미지

	Bullet(const wchar_t* imagePath);
	~Bullet();

	virtual void Update();
	void Move(); //움직이는 함수
	virtual void OnDestroy(); //삭제시 함수
	bool CheckOutOfScreen();

	DWORD GetSpawnTime();

private:
	DWORD SpawnBulletTime;
};

