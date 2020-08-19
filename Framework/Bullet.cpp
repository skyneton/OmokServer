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
	//�Ѿ��� �����ǰ�, �Ѿ��� �̹��� ũ�⸦ ����ؼ�
	//�Ѿ��� �ۿ� ������ �� �������־�� ��.
	int sw = WinApp::GetScreenWidth();					//�������� ���� ����
	int sh = WinApp::GetScreenHeight();					//�������� ���� ����
	int bw = renderer->GetWidth() * transform->scale.x;	//�̹����� ���� ����*������
	int bh = renderer->GetHeight() * transform->scale.y;//�̹����� ���� ����*������

	return transform->position.x<bw * (-1.0f) ||	//�Ѿ��� ���� ���� �����ų�
		transform->position.x>bw + sw ||			//�Ѿ��� ���� ���� �����ų�
		transform->position.y<bh * (-1.0f) ||		//�Ѿ��� ���� ���� �����ų�
		transform->position.y>bh + sh;				//�Ѿ��� �Ʒ� ���� �����ų�
}

void Bullet::OnDestroy() {

}

void Bullet::Update() {
	Move();
}
