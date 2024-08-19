#include "Mode1.h"
#include "Framework.h"
#include "MouseUtil.h"
#include "CameraUtil.h"

std::string Mode1::GameMode1() {

	framework.SetController(Controller, ModeType::Default);
	return __func__;
}

void Mode1::ProcessKeyboard(KeyType Type, KeyState State, unsigned char NormalKey, int SpecialKey) {
	if (Type == KeyType::NormalKey && State == KeyState::Down) {
		switch (NormalKey) {
		case 27: //ESC
			framework.Exit();
			break;
		}
	}

}

void Mode1::ProcessMouse(int Button, int State, int X, int Y) {
}

void Mode1::ProcessMouseWheel(int Button, int Wheel, int X, int Y) {
}



void Mode1::KeyDown(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(KeyType::NormalKey, KeyState::Down, KEY, NULL);
}

void Mode1::KeyUp(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(KeyType::NormalKey, KeyState::Up, KEY, NULL);
}

void Mode1::SpecialKeyDown(int KEY, int X, int Y) {
	ProcessKeyboard(KeyType::SpecialKey, KeyState::Down, NULL, KEY);
}

void Mode1::SpecialKeyUp(int KEY, int X, int Y) {
	ProcessKeyboard(KeyType::SpecialKey, KeyState::Up, NULL, KEY);
}

void Mode1::MouseMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void Mode1::MousePassiveMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void Mode1::Controller() {
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	glutMouseWheelFunc(ProcessMouseWheel);
	glutMouseFunc(ProcessMouse);
	glutSpecialFunc(SpecialKeyDown);
	glutSpecialUpFunc(SpecialKeyUp);
}