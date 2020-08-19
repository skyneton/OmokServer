#pragma once
#include "stdafx.h"
class TimerManager
{
	friend class Framework;
public:
	TimerManager();
	~TimerManager();

private:
	static float deltaTime;
	static void UpdateTime();
public:
	static float GetDeltaTime();
};

