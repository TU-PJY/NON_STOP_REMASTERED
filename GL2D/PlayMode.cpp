#include "PlayMode.h"
#include "Framework.h"
#include "MouseUtil.h"
#include "CameraUtil.h"

#include "CameraController.h"
#include "Player.h"

#include "Ground.h"
#include "GuardRail.h"
#include "BackGround.h"

#include "Target.h"

#include "Scar.h"

std::string Play_Mode::PlayMode() {
	// 카메라 컨트롤러 객체 추가
	framework.AddObject(new CameraController, "camera_controller", Layer::L1);

	// 맵 객체 추가
	framework.AddObject(new BackGround, "background", Layer::L1);
	framework.AddObject(new Ground, "ground", Layer::L1);
	framework.AddObject(new GuardRail, "guardrail", Layer::L3);

	// 플레이어 객체 추가
	framework.AddObject(new Player, "player", Layer::L2);

	// 총 객체 추가
	framework.AddObject(new Scar, "scar", Layer::L2);

	// 조준점 객체 추가
	framework.AddObject(new Target, "target", Layer::L4);

	// 컨트롤러 설정
	framework.SetController(Controller, ModeType::Default);

	glutSetCursor(GLUT_CURSOR_NONE);
	return __func__;
}

void Play_Mode::ProcessKeyboard(KeyType Type, KeyState State, unsigned char NormalKey, int SpecialKey) {
	if (Type == KeyType::NormalKey && State == KeyState::Down) {
		switch (NormalKey) {
		case 27: //ESC
			framework.Exit();
			break;
		}
	}
	
	// 플레이어 키보드 조작
	framework.InputKey("player", Type, State, NormalKey, SpecialKey);
}

void Play_Mode::ProcessMouse(int Button, int State, int X, int Y) {
	// 플레이어 마우스 클릭 조작
	framework.InputMouse("player", Button, State, X, Y);
}

void Play_Mode::ProcessMouseWheel(int Button, int Wheel, int X, int Y) {
}



void Play_Mode::KeyDown(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(KeyType::NormalKey, KeyState::Down, KEY, NULL);
}

void Play_Mode::KeyUp(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(KeyType::NormalKey, KeyState::Up, KEY, NULL);
}

void Play_Mode::SpecialKeyDown(int KEY, int X, int Y) {
	ProcessKeyboard(KeyType::SpecialKey, KeyState::Down, NULL, KEY);
}

void Play_Mode::SpecialKeyUp(int KEY, int X, int Y) {
	ProcessKeyboard(KeyType::SpecialKey, KeyState::Up, NULL, KEY);
}

void Play_Mode::MouseMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
	glutWarpPointer(WIDTH / 2, HEIGHT / 2);
}

void Play_Mode::MousePassiveMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
	glutWarpPointer(WIDTH / 2, HEIGHT / 2);
}

void Play_Mode::Controller() {
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	glutMouseWheelFunc(ProcessMouseWheel);
	glutMouseFunc(ProcessMouse);
	glutSpecialFunc(SpecialKeyDown);
	glutSpecialUpFunc(SpecialKeyUp);
}