#pragma once
#include "GameObject.h"
#include "Bullet.h"
#include "Enemy.h"

class BulletManager :
	public GameObject
{
public:
	std::list<Bullet*> playerBulletList;
	std::list<Bullet*> enemyBulletList;

	void CheckCollision();
	Bullet* PushBackPlayerBullet(Bullet* b);
	Bullet* PushBackEnemyBullet(Bullet* b);
	Enemy* PushBackEnemy(Enemy* enemy);

	virtual void LateUpdate();

	void DestroyPlayerBullet(Bullet* b); //�Ѿ� ���� �˸�
	void DestroyEnemyBullet(Bullet* b); //�Ѿ� ���� �˸�
	void DestroyEnemy(Enemy* e); //�� ���� �˸�

private:
	std::list<Bullet*> destroyedPlayerBullet;
	std::list<Bullet*> destroyedEnemyBullet;
	std::list<Enemy*> destroyedEnemy;
	bool destroyedPlayer;
	void RemoveDestroyed(); //���� ���� ó��

public:
	BulletManager(GameObject* player);
	~BulletManager();
};

