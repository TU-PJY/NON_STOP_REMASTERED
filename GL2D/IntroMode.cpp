#include "Framework.h"
#include "MouseUtil.h"
#include "CameraUtil.h"
#include "IntroMode.h"
#include "IntroScreen.h"

std::string Intro_Mode::IntroMode() {
	framework.AddObject(new IntroScreen, "intro_screen", Layer::L1);
	framework.SetController(Controller, ModeType::Default);
	return __func__;
}

void Intro_Mode::ProcessKeyboard(KeyType Type, KeyState State, unsigned char NormalKey, int SpecialKey) {
	if (auto intro_screen = framework.Find("intro_screen"); intro_screen)
		intro_screen->InputKey(Type, State, NormalKey, SpecialKey);
}

void Intro_Mode::ProcessMouse(int Button, int State, int X, int Y) {
}

void Intro_Mode::ProcessMouseWheel(int Button, int Wheel, int X, int Y) {
}



void Intro_Mode::KeyDown(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(KeyType::NormalKey, KeyState::Down, KEY, NULL);
}

void Intro_Mode::KeyUp(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(KeyType::NormalKey, KeyState::Up, KEY, NULL);
}

void Intro_Mode::SpecialKeyDown(int KEY, int X, int Y) {
	ProcessKeyboard(KeyType::SpecialKey, KeyState::Down, NULL, KEY);
}

void Intro_Mode::SpecialKeyUp(int KEY, int X, int Y) {
	ProcessKeyboard(KeyType::SpecialKey, KeyState::Up, NULL, KEY);
}

void Intro_Mode::MouseMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void Intro_Mode::MousePassiveMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void Intro_Mode::Controller() {
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	glutMouseWheelFunc(ProcessMouseWheel);
	glutMouseFunc(ProcessMouse);
	glutSpecialFunc(SpecialKeyDown);
	glutSpecialUpFunc(SpecialKeyUp);
}
