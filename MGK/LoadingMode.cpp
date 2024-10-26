#include "LoadingMode.h"
#include "Scene.h"
#include "MouseUtil.h"
#include "LoadingScreen.h"

void LoadingMode::Start() {
	Framework::SetBackColor(0.0, 0.0, 0.0);
	scene.AddObject(new LoadingScreen, "loading_screen", 0);
	scene.RegisterController(Controller, MODE_TYPE_DEFAULT);
	scene.RegisterDestructor(Destructor);
	scene.RegisterModeName("LoadingMode");
}

void LoadingMode::Destructor() {}


void LoadingMode::ProcessKeyboard(int State, unsigned char NormalKey, int SpecialKey) {
	if (auto loading_screen = scene.Find("loading_screen"); loading_screen)
		loading_screen->InputKey(State, NormalKey, SpecialKey);
}

void LoadingMode::ProcessMouseButton(int State) {

}

void LoadingMode::ProcessMouseWheel(int State) {

}



void LoadingMode::KeyDown(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(NORMAL_KEY_DOWN, KEY, NULL);
}

void LoadingMode::KeyUp(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(NORMAL_KEY_UP, KEY, NULL);
}

void LoadingMode::SpecialKeyDown(int KEY, int X, int Y) {
	ProcessKeyboard(SPECIAL_KEY_DOWN, NULL, KEY);
}

void LoadingMode::SpecialKeyUp(int KEY, int X, int Y) {
	ProcessKeyboard(SPECIAL_KEY_UP, NULL, KEY);
}

void LoadingMode::MouseMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void LoadingMode::MousePassiveMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void LoadingMode::MouseButton(int Button, int State, int X, int Y) {
	if (State == GLUT_DOWN) {
		if (Button == GLUT_LEFT_BUTTON)
			ProcessMouseButton(LEFT_BUTTON_DOWN);
		else if (Button == GLUT_RIGHT_BUTTON)
			ProcessMouseButton(RIGHT_BUTTON_DOWN);
		else if (Button == GLUT_MIDDLE_BUTTON)
			ProcessMouseButton(MIDDLE_BUTTON_DOWN);
	}
	else if (State == GLUT_UP) {
		if (Button == GLUT_LEFT_BUTTON)
			ProcessMouseButton(LEFT_BUTTON_UP);
		else if (Button == GLUT_RIGHT_BUTTON)
			ProcessMouseButton(RIGHT_BUTTON_UP);
		else if (Button == GLUT_MIDDLE_BUTTON)
			ProcessMouseButton(MIDDLE_BUTTON_UP);
	}
}

void LoadingMode::MouseWheel(int Button, int Wheel, int X, int Y) {
	if (Wheel > 0)
		ProcessMouseWheel(WHEEL_UP);
	else if (Wheel < 0)
		ProcessMouseWheel(WHEEL_DOWN);
}

void LoadingMode::Controller() {
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	glutMouseWheelFunc(MouseWheel);
	glutMouseFunc(MouseButton);
	glutSpecialFunc(SpecialKeyDown);
	glutSpecialUpFunc(SpecialKeyUp);
}
