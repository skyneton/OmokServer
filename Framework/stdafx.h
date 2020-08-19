
#include <Windows.h>
#include <iostream>
#include <string>
#include <list>

//다이렉트x 관련 헤더
#include <d2d1.h>
#include <d2d1helper.h>

//COM
#include <wincodec.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "WindowsCodecs.lib")
#pragma comment(lib, "Winmm.lib")


#define SAFE_RELEASE(p) { if(p){p->Release();} p=NULL; }
#define SAFE_DELETE(p) {if(p!=nullptr){delete(p);} p=nullptr; }

typedef D2D1_POINT_2F Point;


#define CHANGESCENE(scene) { Framework::GetInstance().GetSceneManager().GetCurrentScene().ChangeScene(scene) }
#define DESTROY(gameObject) {Framework::GetInstance().GetSceneManager().GetCurrentScene().Destroy(gameObject) }