#pragma once
#include "GameObject.h"
#include "BulletManager.h"
#include "RadiusCollider.h"


class Player : public GameObject
{
	
public:
	Player(const wchar_t* imagePath, float hp);
	~Player();

	BulletManager* bm;

	float moveSpeed;

	virtual void Update();
	void Move();
	void Shoot();
	void Healing();


	RadiusCollider* col;
	bool Hit(float damage);
	void CheckOutOfScreen();
	void StageChange(int stage);
	static int GetStage();
	static int GetHealingChance();
	void SetHealingChance(int n);
	void ShootLargeBullet();
	void SetMaxHP(float hp);

	static int largeBulletChance;
	void HPChange(float hp);
	void HPBarChange();

private:
	float moveLock;
	DWORD shootDelay;
	DWORD dashDelay;
	DWORD dashTimer;
	DWORD doubleClickUPDelay;
	DWORD doubleClickLeftDelay;
	DWORD doubleClickDownDelay;
	DWORD doubleClickRightDelay;
	static int stage;
	static int healingChance;
	float MaxHP;
	bool dash_status;
	bool dash_once;
	float hp;
	GameObject* hp_bar;
};

