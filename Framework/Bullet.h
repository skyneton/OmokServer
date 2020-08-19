#pragma once
#include "GameObject.h"
#include "RadiusCollider.h"
class Bullet :
	public GameObject
{
public:
	RadiusCollider* col;
	float speed, speedRate; //�ӵ�, ���ӵ�
	float angle, angleRate; //����, ���ӵ�
	float damage; //������

	Bullet(const wchar_t* imagePath);
	~Bullet();

	virtual void Update();
	void Move(); //�����̴� �Լ�
	virtual void OnDestroy(); //������ �Լ�
	bool CheckOutOfScreen();

	DWORD GetSpawnTime();

private:
	DWORD SpawnBulletTime;
};

