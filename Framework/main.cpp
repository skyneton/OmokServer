#include "stdafx.h"
#include "Framework.h"
#include "GameScene.h"

int main()
{
	Framework* f = new Framework;
	f->Run(new GameScene(),L"≈∏¿Ã∆≤", 1280, 800, false);
	SAFE_DELETE(f);

	return 0;
}