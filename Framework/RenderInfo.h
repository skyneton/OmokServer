#pragma once
#include "D2DApp.h"
#include "Component.h"
#include "Transform.h"

class RenderInfo :
	public Component
{
public:
	RenderInfo();
	RenderInfo(Sprite* startSprite, float alpha = 1.0f);
private:
	Sprite* currentSprite;
	float alpha;
	bool initialized;
	int width, height;

public:
	Sprite* GetCurrentSprite();
	float GetAlpha();
	bool GetInitialized();
	int GetWidth();
	int GetHeight();
	virtual void Update() {}
};

