#include "stdafx.h"
#include "Player.h"
#include "InputManager.h"
#include "TimerManager.h"
#include "Bullet.h"
#include "Scene.h"

#define TELEPORT_AMOUNT 180
#define DOUBLE_CLICK 250
#define PI 3.141592653589793f
#define SHOOT_MOUSE_LOC false
#define DASH_COOL 6000
#define DASH_START 4000
#define DASH_AMOUNT 2.f
#define LARGE_BULLET true

int Player::stage;
int Player::healingChance;
int Player::largeBulletChance;

Player::Player(const wchar_t* imagePath, float hp)
:GameObject(imagePath), hp(hp - 20.f)
{
	col = new RadiusCollider(transform, renderer->GetWidth() * 0.5f);
	MaxHP = hp - 20.f;
	moveSpeed = 180.f;
	moveLock = 5;
	bm = new BulletManager(this);
	Scene::GetCurrentScene()->PushBackGameObject(bm);

	hp_bar = Scene::GetCurrentScene()->PushBackGameObject(new GameObject(L"resources/hp_bar.png"));

	SetHealingChance(0);
	StageChange(1);
}


Player::~Player()
{
}

void Player::HPChange(float h) {
	hp = h;
	HPBarChange();
}

void Player::HPBarChange() {
	hp_bar->transform->SetScale(hp / MaxHP, 0.5f);
	hp_bar->transform->position.y = hp_bar->renderer->GetHeight() / 2.0f + 3.f;
	hp_bar->transform->position.x = hp_bar->renderer->GetWidth() * hp_bar->transform->scale.x / 2.0f + 10.f;
}

void Player::StageChange(int i) {
	stage = i;
	SetHealingChance(GetHealingChance() + 1);
	MaxHP += 20.f;
	HPChange(hp + 20.f);

	if (!(i % 3)) largeBulletChance++;

	printf("Stage Changed: %d\n", stage);

	for (int i = 0; i < stage; i++) {
		Enemy* enemy = bm->PushBackEnemy(new Enemy(L"resources/enemy.png", stage*50.f, this));
		Scene::GetCurrentScene()->PushBackGameObject(enemy);
		enemy->transform->SetPosition(WinApp::GetScreenWidth() / 2, 100.0f);
		enemy->transform->SetScale(0.25f, 0.25f);
	}
}

int Player::GetHealingChance() {
	return healingChance;
}

int Player::GetStage() {
	return stage;
}

void Player::SetHealingChance(int n) {
	healingChance = n;
	printf("HealingChance: %d\n", healingChance);
}

void Player::Update() {
	//매 프레임 호출
	Move();
	Shoot();
	Healing();
	if (LARGE_BULLET)
		ShootLargeBullet();
}

void Player::ShootLargeBullet() {
	if (InputManager::GetKeyDown('E') && largeBulletChance > 0) {
		largeBulletChance--;
		printf("남은 대형 총알: %d\n", largeBulletChance);

		float myY = transform->position.y;
		float myX = transform->position.x;
		POINT mousePos;
		GetCursorPos(&mousePos);
		ScreenToClient(InputManager::winApp->GetHWND(), &mousePos);


		Bullet* b = new Bullet(L"resources/arrow1.png");
		b->damage = 2000.f * stage;
		b->speed = 700.0f;

		b->transform->SetScale(2.f, 2.f);
		Scene::GetCurrentScene()->PushBackGameObject(b);
		bm->PushBackPlayerBullet(b);

		b->transform->position = this->transform->position;

		b->angle = atan2f(myY - mousePos.y, myX - mousePos.x) / (2.0f * PI) + 0.5f;
	}
}

void Player::Healing() {
	if (InputManager::GetKeyDown('F') && healingChance > 0) {
		printf("Player HP Healing: %.2lf -> %.2lf\n", hp, MaxHP);
		hp = MaxHP;
		SetHealingChance(GetHealingChance() - 1);

		HPBarChange();
	}
}

bool Player::Hit(float damage) {
	hp -= damage;
	printf("PlayerDamaged: %.2lf -> %.2lf\n", hp + damage, hp);
	HPBarChange();

	return hp <= 0;
}

void Player::SetMaxHP(float hp) {
	MaxHP = hp;
}

void Player::Move() {
	//움직임을 담당
	float y = transform->position.y;
	float x = transform->position.x;

	DWORD now = timeGetTime();

	float dash = 1.0f;

	if (now - dashDelay >= DASH_COOL && !dash_status) {
		dash_status = true;
		printf("CAN DASH\n");
		dashTimer = now;
		dash_once = false;
	}

	if (dash_status && dash_once && now - dashTimer > DASH_START) {
		dash_status = false;
		dashDelay = now;
		printf("CAN'T DASH\n");
	}

	if (dash_status && InputManager::GetKeyDown(VK_SHIFT)) {
		dashTimer = now;
	}

	if (dash_status && InputManager::GetKeyState(VK_SHIFT)) {
		if (now - dashTimer <= DASH_START) {
			dash = DASH_AMOUNT + stage * 0.3f;
			dash_once = true;
		}
		else {
			dash_status = false;
			dashDelay = now;
			printf("CAN't DASH\n");
		}
	}

	if (InputManager::GetKeyState('W') || InputManager::GetKeyState(VK_UP)) {
		transform->position.y -= moveSpeed * dash * TimerManager::GetDeltaTime();

		if (InputManager::GetKeyDown('W') || InputManager::GetKeyDown(VK_UP)) {
			if (now - doubleClickUPDelay <= DOUBLE_CLICK) {
				transform->position.y -= TELEPORT_AMOUNT * dash;
			}
			doubleClickUPDelay = now;
		}
	}

	if (InputManager::GetKeyState('S') || InputManager::GetKeyState(VK_DOWN)) {
		transform->position.y += moveSpeed * dash * TimerManager::GetDeltaTime();

		if (InputManager::GetKeyDown('S') || InputManager::GetKeyDown(VK_DOWN)) {
			if (now - doubleClickDownDelay <= DOUBLE_CLICK) {
				transform->position.y += TELEPORT_AMOUNT * dash;
			}
			doubleClickDownDelay = now;
		}
	}

	if (InputManager::GetKeyState('A') || InputManager::GetKeyState(VK_LEFT)) {
		transform->position.x -= moveSpeed * dash * TimerManager::GetDeltaTime();

		if (InputManager::GetKeyDown('A') || InputManager::GetKeyDown(VK_LEFT)) {
			if (now - doubleClickLeftDelay <= DOUBLE_CLICK) {
				transform->position.x -= TELEPORT_AMOUNT * dash;
			}
			doubleClickLeftDelay = now;
		}
	}

	if (InputManager::GetKeyState('D') || InputManager::GetKeyState(VK_RIGHT)) {
		transform->position.x += moveSpeed * dash * TimerManager::GetDeltaTime();

		if (InputManager::GetKeyDown('D') || InputManager::GetKeyDown(VK_RIGHT)) {
			if (now - doubleClickRightDelay <= DOUBLE_CLICK) {
				transform->position.x += TELEPORT_AMOUNT * dash;
			}
			doubleClickRightDelay = now;
		}
	}

	CheckOutOfScreen();

	if (y == transform->position.y && x == transform->position.x) {
		if (moveLock > 0) {
			transform->position.y += moveLock * 7 * TimerManager::GetDeltaTime();
			if (moveLock < 1)
				moveLock -= 0.05f;
			else
				moveLock -= 0.1f;
			if (moveLock < 0) moveLock = -5;
		}
		else {
			transform->position.y += moveLock * 7 * TimerManager::GetDeltaTime();
			if (moveLock > -1)
				moveLock += 0.05f;
			else
				moveLock += 0.1f;
			if (moveLock > 0) moveLock = 5;
		}
	}
	else moveLock = 5;
}

void Player::CheckOutOfScreen()
{
	//총알의 포지션과, 총알의 이미지 크기를 고려해서
	//총알이 밖에 나갔는 지 판정해주어야 함.
	int sw = WinApp::GetScreenWidth();					//윈도우의 가로 길이
	int sh = WinApp::GetScreenHeight();					//윈도우의 세로 길이

	if (transform->position.x <= 0)	//플레이어가 왼쪽 끝을 나갔거나
		transform->position.x = 0;

	if (transform->position.x >= sw)		//플레이어가 오른 끝을 나갔거나
		transform->position.x = sw;

	if (transform->position.y <= 0)	//플레이어가 위쪽 끝을 나갔거나
		transform->position.y = 0;

	if (transform->position.y >= sh)		//플레이어가 아래 끝을 나갔거나
		transform->position.y = sh;
}

void Player::Shoot() {
	//총알 발사
	if (InputManager::GetKeyState(VK_SPACE)) {
		DWORD delayCheck = timeGetTime();
		if (delayCheck - shootDelay >= 400 || InputManager::GetKeyDown(VK_SPACE)) { //0.4초마다 총알 발사
			float myY = transform->position.y;
			float myX = transform->position.x;
			POINT mousePos;
			GetCursorPos(&mousePos);
			ScreenToClient(InputManager::winApp->GetHWND(), &mousePos);

			shootDelay = delayCheck;
			Bullet* b;
			srand(timeGetTime());
			if (rand() % 101 <= 70) {
				b = new Bullet(L"resources/arrow2.png");
				b->damage = 5.0f * stage;
				b->speed = 1500.0f;
			}
			else {
				b = new Bullet(L"resources/arrow1.png");
				b->damage = 6.5f * stage;
				b->speed = 1000.0f;
			}
			b->transform->SetScale(0.1f, 0.1f);
			Scene::GetCurrentScene()->PushBackGameObject(b);
			bm->PushBackPlayerBullet(b);

			b->transform->position = this->transform->position;
			if (!SHOOT_MOUSE_LOC)
				b->transform->position.y -= this->transform->scale.y * this->renderer->GetHeight() / 2.0f;
			// b->angleRate = 0.2f;
			b->angle = 0.75f;
			if (SHOOT_MOUSE_LOC)
				b->angle = atan2f(myY - mousePos.y, myX - mousePos.x) / (2.0f * PI) + 0.5f;
			else
				b->angle = 0.75f;
			b->transform->SetRotation(90.0f + b->angle);
		}
	}
}
