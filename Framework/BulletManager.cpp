#include "stdafx.h"
#include "BulletManager.h"
#include "Scene.h"
#include "Player.h"
#include "GameScene.h"

Player* player;

BulletManager::BulletManager(GameObject* pl)
{
	player = (Player*) pl;
}


BulletManager::~BulletManager()
{
}

Bullet* BulletManager::PushBackEnemyBullet(Bullet* b) {
	enemyBulletList.push_back(b);
	return b;
}

Bullet* BulletManager::PushBackPlayerBullet(Bullet* b) {
	playerBulletList.push_back(b);
	return b;
}

Enemy* BulletManager::PushBackEnemy(Enemy* b) {
	GameScene::enemy.push_back(b);
	return b;
}

void BulletManager::DestroyPlayerBullet(Bullet* b) {
	destroyedPlayerBullet.push_back(b);
}

void BulletManager::DestroyEnemyBullet(Bullet* b) {
	destroyedEnemyBullet.push_back(b);
}

void BulletManager::DestroyEnemy(Enemy* e) {
	destroyedEnemy.push_back(e);
}

void BulletManager::RemoveDestroyed() {
	if (destroyedPlayer) {
		destroyedPlayerBullet = playerBulletList;
		destroyedEnemyBullet = enemyBulletList;
		destroyedEnemy = GameScene::enemy;
	}

	for (auto& i : destroyedPlayerBullet) {
		playerBulletList.remove(i);
		Scene::GetCurrentScene()->Destroy(i);
	}
	for (auto& i : destroyedEnemyBullet) {
		enemyBulletList.remove(i);
		Scene::GetCurrentScene()->Destroy(i);
	}
	for (auto& i : destroyedEnemy) {
		GameScene::enemy.remove(i);
		Scene::GetCurrentScene()->Destroy(i);
	}

	if (GameScene::enemy.size() == 0) {
		if (destroyedPlayer) {
			player->SetHealingChance(0);
			player->SetMaxHP(180.0f);
			player->HPChange(180.0f);
			player->largeBulletChance = 0;
			player->StageChange(1);
			player->transform->SetPosition(WinApp::GetScreenWidth() / 2, 450.f);
			destroyedPlayer = false;
		}
		else player->StageChange(player->GetStage() + 1);
	}

	destroyedPlayerBullet.clear();
	destroyedEnemyBullet.clear();
	destroyedEnemy.clear();
}

void BulletManager::LateUpdate() {
	CheckCollision();
	RemoveDestroyed();
}

void BulletManager::CheckCollision() {
	DWORD now = timeGetTime();
	for (auto& i : playerBulletList) {
		if (i->CheckOutOfScreen() || now - i->GetSpawnTime() >= 4000 + 300 * player->GetStage()) {
			DestroyPlayerBullet(i);
		}
		else {
			for (auto& enemy : GameScene::enemy) {
				if (i->col->Intersected(enemy->col)) {
					if (enemy->hp < i->damage) {
						i->damage -= enemy->hp;
					}
					else
						DestroyPlayerBullet(i);
					if (enemy->Hit(i->damage)) {
						DestroyEnemy(enemy);
					}

					break;
				}
			}
		}
	}

	for (auto& i : enemyBulletList) {
		if (i->CheckOutOfScreen() || now - i->GetSpawnTime() >= 4000 + 300 * player->GetStage())
			DestroyEnemyBullet(i);
		else if (i->col->Intersected(player->col)) {
			if (player->Hit(i->damage)) {
				destroyedPlayer = true;
				break;
			}
			DestroyEnemyBullet(i);
		}
	}

	for (auto& p : playerBulletList) {
		for (auto& e : enemyBulletList) {
			if (p->col->Intersected(e->col)) {
				DestroyEnemyBullet(e);
				if (p->damage > e->damage) {
					p->damage -= e->damage;
				}else
					DestroyPlayerBullet(p);
				break;
			}
		}
	}
}
