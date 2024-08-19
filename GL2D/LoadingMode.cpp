#include "Framework.h"
#include "MouseUtil.h"
#include "CameraUtil.h"
#include "LoadingMode.h"
#include "LoadingScreen.h"

std::string Loading_Mode::LoadingMode() {
	framework.AddObject(new LoadingScreen, "loading_screen", Layer::L1);
	framework.SetController(Controller, ModeType::Default);
	return __func__;
}

void Loading_Mode::ProcessKeyboard(KeyType Type, KeyState State, unsigned char NormalKey, int SpecialKey) {
	if (Type == KeyType::NormalKey && State == KeyState::Down) {
		switch (NormalKey) {
		case 27: //ESC
			framework.Exit();
			break;
		}
	}

}

void Loading_Mode::ProcessMouse(int Button, int State, int X, int Y) {
}

void Loading_Mode::ProcessMouseWheel(int Button, int Wheel, int X, int Y) {
}



void Loading_Mode::KeyDown(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(KeyType::NormalKey, KeyState::Down, KEY, NULL);
}

void Loading_Mode::KeyUp(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(KeyType::NormalKey, KeyState::Up, KEY, NULL);
}

void Loading_Mode::SpecialKeyDown(int KEY, int X, int Y) {
	ProcessKeyboard(KeyType::SpecialKey, KeyState::Down, NULL, KEY);
}

void Loading_Mode::SpecialKeyUp(int KEY, int X, int Y) {
	ProcessKeyboard(KeyType::SpecialKey, KeyState::Up, NULL, KEY);
}

void Loading_Mode::MouseMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void Loading_Mode::MousePassiveMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void Loading_Mode::Controller() {
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	glutMouseWheelFunc(ProcessMouseWheel);
	glutMouseFunc(ProcessMouse);
	glutSpecialFunc(SpecialKeyDown);
	glutSpecialUpFunc(SpecialKeyUp);
}
