#include "LobbyMode.h"
#include "Scene.h"
#include "MouseUtil.h"
#include "PlayerInfo.h"

// 로비 모드

void LobbyMode::Start() {
	Framework::SetBackColor(0.5, 0.5, 0.5);
	
	scene.AddObject(new PlayerInfo, "player_info", LAYER_1);
	scene.RegisterController(Controller, MODE_TYPE_DEFAULT);
	scene.RegisterDestructor(Destructor);
	scene.RegisterModeName("LobbyMode");
}

void LobbyMode::Destructor() {

}

void LobbyMode::ProcessKeyboard(int State, unsigned char NormalKey, int SpecialKey) {
	if (State == NORMAL_KEY_DOWN) {
		switch (NormalKey) {
		case 27: //ESC
			scene.Exit();
			break;
		}
	}

	scene.InputKey("player_info", State, NormalKey, SpecialKey);
}

void LobbyMode::ProcessMouseButton(int State) {
}

void LobbyMode::ProcessMouseWheel(int State) {
}



void LobbyMode::KeyDown(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(NORMAL_KEY_DOWN, KEY, NULL);
}

void LobbyMode::KeyUp(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(NORMAL_KEY_UP, KEY, NULL);
}

void LobbyMode::SpecialKeyDown(int KEY, int X, int Y) {
	ProcessKeyboard(SPECIAL_KEY_DOWN, NULL, KEY);
}

void LobbyMode::SpecialKeyUp(int KEY, int X, int Y) {
	ProcessKeyboard(SPECIAL_KEY_UP, NULL, KEY);
}

void LobbyMode::MouseMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void LobbyMode::MousePassiveMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void LobbyMode::MouseWheel(int Button, int Wheel, int X, int Y) {
	if (Wheel > 0)
		ProcessMouseWheel(WHEEL_UP);
	else if (Wheel < 0)
		ProcessMouseWheel(WHEEL_DOWN);
}

void LobbyMode::MouseButton(int Button, int State, int X, int Y) {
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

void LobbyMode::Controller() {
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	glutMouseWheelFunc(MouseWheel);
	glutMouseFunc(MouseButton);
	glutSpecialFunc(SpecialKeyDown);
	glutSpecialUpFunc(SpecialKeyUp);
}