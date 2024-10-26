#include "IntroMode.h"
#include "Scene.h"
#include "MouseUtil.h"
#include "IntroScreen.h"

void IntroMode::Start() {
	scene.AddObject(new IntroScreen, "intro_screen", 0);
	scene.RegisterController(Controller, MODE_TYPE_DEFAULT);
	scene.RegisterDestructor(Destructor);
	scene.RegisterModeName("IntroMode");
}

void IntroMode::Destructor() {
	soundUtil.Release(IntroSound);
	imageUtil.Release(ImageEngineLogo);
	imageUtil.Release(ImageFMODLogo);
}

void IntroMode::ProcessKeyboard(int State, unsigned char NormalKey, int SpecialKey) {
	if (auto intro_screen = scene.Find("intro_screen"); intro_screen)
		intro_screen->InputKey(State, NormalKey, SpecialKey);
}

void IntroMode::ProcessMouseButton(int State) {

}

void IntroMode::ProcessMouseWheel(int State) {

}



void IntroMode::KeyDown(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(NORMAL_KEY_DOWN, KEY, NULL);
}

void IntroMode::KeyUp(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(NORMAL_KEY_UP, KEY, NULL);
}

void IntroMode::SpecialKeyDown(int KEY, int X, int Y) {
	ProcessKeyboard(SPECIAL_KEY_DOWN, NULL, KEY);
}

void IntroMode::SpecialKeyUp(int KEY, int X, int Y) {
	ProcessKeyboard(SPECIAL_KEY_UP, NULL, KEY);
}

void IntroMode::MouseMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void IntroMode::MousePassiveMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void IntroMode::MouseButton(int Button, int State, int X, int Y) {
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

void IntroMode::MouseWheel(int Button, int Wheel, int X, int Y) {
	if (Wheel > 0)
		ProcessMouseWheel(WHEEL_UP);
	else if (Wheel < 0)
		ProcessMouseWheel(WHEEL_DOWN);
}

void IntroMode::Controller() {
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	glutMouseWheelFunc(MouseWheel);
	glutMouseFunc(MouseButton);
	glutSpecialFunc(SpecialKeyDown);
	glutSpecialUpFunc(SpecialKeyUp);
}
