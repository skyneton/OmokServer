#include "stdafx.h"
#include "GameScene.h"

std::list<Enemy*> GameScene::enemy;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	background = PushBackGameObject(new GameObject(L"resources/background.png"));
	background->transform->SetPosition(WinApp::GetScreenWidth() / 2.f, WinApp::GetScreenHeight() / 2.f);
	background->transform->SetScale((float)WinApp::GetScreenWidth() / (float)background->renderer->GetWidth(), (float)WinApp::GetScreenHeight() / (float) background->renderer->GetHeight());

	player = (Player*)PushBackGameObject(new Player(L"resources/player.png", 200.f));
	player->transform->SetPosition(WinApp::GetScreenWidth() / 2, 450.f);
	player->transform->SetScale(0.3f, 0.3f);
}
