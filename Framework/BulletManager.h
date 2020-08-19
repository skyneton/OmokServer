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

	void DestroyPlayerBullet(Bullet* b); //총알 삭제 알림
	void DestroyEnemyBullet(Bullet* b); //총알 삭제 알림
	void DestroyEnemy(Enemy* e); //적 삭제 알림

private:
	std::list<Bullet*> destroyedPlayerBullet;
	std::list<Bullet*> destroyedEnemyBullet;
	std::list<Enemy*> destroyedEnemy;
	bool destroyedPlayer;
	void RemoveDestroyed(); //실제 삭제 처리

public:
	BulletManager(GameObject* player);
	~BulletManager();
};

