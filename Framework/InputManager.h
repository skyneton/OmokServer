#pragma once
#include "WinApp.h"
#define KEY_PRESSED 2
#define KEY_DOWN 1
#define KEY_UP -1
#define KEY_NOT_PRESSED 0

class InputManager
{
	friend class Framework;
	friend class Player;
public:
	InputManager();
	~InputManager();

private:
	static WinApp* winApp;
	static bool lkey[256], rkey[256];
	static int mX, mY;

public:
	static void UpdateKeyState();

	static int GetKeyState(int vk);

	static bool GetKeyDown(int vk);
	static bool GetKeyPressed(int vk);
	static bool GetKeyUp(int vk);

	static int GetMouseX();
	static int GetMouseY();
};

