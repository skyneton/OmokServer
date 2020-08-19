#pragma once
#include "GameObject.h"
#include "RadiusCollider.h"

class Enemy :
	public GameObject
{
public:
	float hp;
	RadiusCollider* col;
	Enemy(const wchar_t* imagePath, float hp, GameObject* player);
	~Enemy();
	
	
	bool Hit(float damage);
	virtual void Update();
	void CheckOutOfScreen();

private:
	int verti, hori;
	DWORD shootDelay;
	void Move();
	void Shoot();

	GameObject* player;
};
