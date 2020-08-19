#include "stdafx.h"
#include "Bullet.h"
#include "TimerManager.h"
#include "RadiusCollider.h"
#define PI 3.141592653589793f

Bullet::Bullet(const wchar_t* imagePath) :GameObject(imagePath)
{
	col = new RadiusCollider(transform, renderer->GetWidth() * 0.5f);
	speed = 0.f;
	speedRate = 0.f;
	angle = 0.f;
	angleRate = 0.f;
	damage = 0.f;
	SpawnBulletTime = timeGetTime();
}

DWORD Bullet::GetSpawnTime() {
	return SpawnBulletTime;
}


Bullet::~Bullet()
{
	SAFE_DELETE(col);
}

void Bullet::Move() {
	float rad = PI * angle * 2.0f;

	transform->position.x += (speed * cosf(rad) * TimerManager::GetDeltaTime());
	transform->position.y += (speed * sinf(rad) * TimerManager::GetDeltaTime());

	speed += speedRate * TimerManager::GetDeltaTime();
	angle += angleRate * TimerManager::GetDeltaTime();


}

bool Bullet::CheckOutOfScreen()
{
	//총알의 포지션과, 총알의 이미지 크기를 고려해서
	//총알이 밖에 나갔는 지 판정해주어야 함.
	int sw = WinApp::GetScreenWidth();					//윈도우의 가로 길이
	int sh = WinApp::GetScreenHeight();					//윈도우의 세로 길이
	int bw = renderer->GetWidth() * transform->scale.x;	//이미지의 가로 길이*스케일
	int bh = renderer->GetHeight() * transform->scale.y;//이미지의 세로 길이*스케일

	return transform->position.x<bw * (-1.0f) ||	//총알이 왼쪽 끝을 나갔거나
		transform->position.x>bw + sw ||			//총알이 오른 끝을 나갔거나
		transform->position.y<bh * (-1.0f) ||		//총알이 위쪽 끝을 나갔거나
		transform->position.y>bh + sh;				//총알이 아래 끝을 나갔거나
}

void Bullet::OnDestroy() {

}

void Bullet::Update() {
	Move();
}
