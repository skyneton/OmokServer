#include "stdafx.h"
#include "TimerManager.h"

float TimerManager::deltaTime;

TimerManager::TimerManager()
{
}


TimerManager::~TimerManager()
{
}

void TimerManager::UpdateTime() {
	static DWORD oldTime = timeGetTime();
	DWORD curTime = timeGetTime();
	deltaTime = (float) (curTime - oldTime) * 0.001f;
	oldTime = curTime;
}

float TimerManager::GetDeltaTime() {
	return deltaTime;
}
