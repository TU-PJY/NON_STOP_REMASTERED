#include "PlayMode.h"
#include "Scene.h"
#include "MouseUtil.h"

// �߰��� �ʿ��� ������Ʈ�� ��������� �����Ѵ�.
#include "Player.h"
#include "BackGround.h"
#include "Crosshair.h"
#include "SCAR_H.h"

void PlayMode::Start() {
	// ������ ���� ������ �ٲ۴�.
	Framework::SetBackColorRGB(166, 166, 166);

	// Ŀ�� ����
	glutSetCursor(GLUT_CURSOR_NONE);

	// playmode�� �ʿ��� ���ҽ����� �ε��Ѵ�.
	imageUtil.Import(BackGroundImage, "res//BackGround_wide.png", IMAGE_TYPE_NEAREST);
	imageUtil.Import(SCAR_H_Image, "res//SCAR_H_right.png", IMAGE_TYPE_NEAREST);
	imageUtil.Import(PlayerImage, "res//commando_right.png", IMAGE_TYPE_NEAREST);
	imageUtil.Import(FlameImage, "res//flame_right.png", IMAGE_TYPE_NEAREST);

	soundUtil.Import(SCAR_H_Shoot, "res//sounds//scar_shoot.ogg", FMOD_DEFAULT);

	// playmode�� �ʿ��� ������Ʈ���� scene�� �߰��Ѵ�.
	scene.AddObject(new BackGround, "background", LAYER_1);
	scene.AddObject(new Crosshair, "crosshair", LAYER_4);
	scene.AddObject(new Player, "player", LAYER_3);
	scene.AddObject(new SCAR_H(0.13), "SCAR_H", LAYER_3);

	scene.RegisterController(Controller, MODE_TYPE_DEFAULT);
	scene.RegisterDestructor(Destructor);
	scene.RegisterModeName("PlayMode");
}

void PlayMode::Destructor() {
	// ��尡 ������ �ε��ߴ� ���ҽ��� �޸𸮿��� �����Ѵ�.
	imageUtil.Release(BackGroundImage);
	imageUtil.Release(PlayerImage);
}

void PlayMode::ProcessKeyboard(int State, unsigned char NormalKey, int SpecialKey) {
	// ���� �ÿ��� Ȱ��ȭ �ص�. ��� ���� ����
	if (State == NORMAL_KEY_DOWN) {
		switch (NormalKey) {
		case NK_ESCAPE:
			scene.Exit();
			break;
		}
	}

	// ��Ʈ�ѷ� �Է��� �ʿ��� ��ü�� ��Ʈ�ѷ��� �����Ѵ�.
	// Start()���� �߰��� �� �Է��ߴ� ������Ʈ �±׷� ������Ʈ�� �����͸� ã�´�.
	if (auto player = scene.Find("player"); player)
		player->InputKey(State, NormalKey, SpecialKey);
}

void PlayMode::ProcessMouseButton(int State) {
	if (auto player = scene.Find("player"); player)
		player->InputMouse(State);
}

void PlayMode::ProcessMouseWheel(int State) {
}



void PlayMode::KeyDown(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(NORMAL_KEY_DOWN, KEY, NULL);
}

void PlayMode::KeyUp(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(NORMAL_KEY_UP, KEY, NULL);
}

void PlayMode::SpecialKeyDown(int KEY, int X, int Y) {
	ProcessKeyboard(SPECIAL_KEY_DOWN, NULL, KEY);
}

void PlayMode::SpecialKeyUp(int KEY, int X, int Y) {
	ProcessKeyboard(SPECIAL_KEY_UP, NULL, KEY);
}

void PlayMode::MouseMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void PlayMode::MousePassiveMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void PlayMode::MouseWheel(int Button, int Wheel, int X, int Y) {
	if (Wheel > 0)
		ProcessMouseWheel(WHEEL_UP);
	else if (Wheel < 0)
		ProcessMouseWheel(WHEEL_DOWN);
}

void PlayMode::MouseButton(int Button, int State, int X, int Y) {
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

void PlayMode::Controller() {
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	glutMouseWheelFunc(MouseWheel);
	glutMouseFunc(MouseButton);
	glutSpecialFunc(SpecialKeyDown);
	glutSpecialUpFunc(SpecialKeyUp);
}