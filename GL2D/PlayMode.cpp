#include "PlayMode.h"
#include "Framework.h"
#include "MouseUtil.h"
#include "CameraUtil.h"

#include "Player.h"

std::string Play_Mode::PlayMode() {
	// 플레이어 객체 추가
	framework.AddObject(new Player, "player", Layer::L2);

	framework.SetController(Controller, ModeType::Default);
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
	if (auto Player = framework.Find("player"); Player)
		Player->InputKey(Type, State, NormalKey, SpecialKey);
}

void Play_Mode::ProcessMouse(int Button, int State, int X, int Y) {
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
}

void Play_Mode::MousePassiveMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
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