#include "PlayMode.h"
#include "Scene.h"
#include "MouseUtil.h"

// 추가가 필요한 오브젝트의 헤더파일을 포함한다.
#include "Player.h"
#include "BackGround.h"
#include "Crosshair.h"
#include "MonsterGenerator.h"
#include "UIProps.h"

void PlayMode::Start() {
	// 배경색을 밝은 색으로 바꾼다.
	Framework::SetBackColorRGB(166, 166, 166);

	// 커서 숨김
	glutSetCursor(GLUT_CURSOR_NONE);

	// playmode에 필요한 리소스들을 로드한다.
	// 이미지
	imageUtil.Import(BackGroundImage, "res//BackGround_wide.png", IMAGE_TYPE_NEAREST);
	imageUtil.Import(SCAR_H_Image, "res//SCAR_H_right.png", IMAGE_TYPE_NEAREST);
	imageUtil.Import(PlayerImage, "res//commando_right.png", IMAGE_TYPE_NEAREST);
	imageUtil.Import(FlameImage, "res//flame_right.png", IMAGE_TYPE_NEAREST);
	imageUtil.Import(RegularMonsterImage, "res//monster_right.png", IMAGE_TYPE_NEAREST);
	imageUtil.Import(IndicatorBackImage, "res//indicator_back.png", IMAGE_TYPE_NEAREST);

	// 사운드
	soundUtil.Import(SCAR_H_Shoot, "res//sounds//scar_shoot.ogg", FMOD_DEFAULT);
	soundUtil.Import(AR_Reload, "res//sounds//ar_reload.ogg", FMOD_DEFAULT);

	// 개발 시에는 여기다가 플레이어 총 이름을 정의하나, 나중에 메인 화면 모드에서 정의하도록 할 예정이다
	PlayerGunName = "SCAR_H";

	// playmode에 필요한 오브젝트들을 scene에 추가한다.
	// UI 오브젝트 및 무형 오브젝트
	scene.AddObject(new MonsterGenerator, "monster_generator", LAYER_1);
	scene.AddObject(new IndBack, "indback", LAYER_5);
	scene.AddObject(new IndAmmo, "indammo", LAYER_5);
	scene.AddObject(new Crosshair, "crosshair", LAYER_4);

	// 일반 오브젝트
	scene.AddObject(new BackGround, "background", LAYER_1);
	scene.AddObject(new Player(PlayerGunName), "player", LAYER_3);

	scene.RegisterController(Controller, MODE_TYPE_DEFAULT);
	scene.RegisterDestructor(Destructor);
	scene.RegisterModeName("PlayMode");
}

void PlayMode::Destructor() {
	// 모드가 끝나면 로드했던 리소스를 메모리에서 해제한다.
	// 이미지
	imageUtil.Release(BackGroundImage);
	imageUtil.Release(PlayerImage);
	imageUtil.Release(SCAR_H_Image);
	imageUtil.Release(FlameImage);
	imageUtil.Release(IndicatorBackImage);

	// 사운드
	soundUtil.Release(SCAR_H_Shoot);
	soundUtil.Release(AR_Reload);
}

void PlayMode::ProcessKeyboard(int State, unsigned char NormalKey, int SpecialKey) {
	// 개발 시에는 활성화 해둠. 즉시 종료 역할
	if (State == NORMAL_KEY_DOWN) {
		switch (NormalKey) {
		case NK_ESCAPE:
			scene.Exit();
			break;
		}
	}

	// 컨트롤러 입력이 필요한 객체에 컨트롤러를 연결한다.
	// Start()에서 추가할 때 입력했던 오브젝트 태그로 오브젝트의 포인터를 찾는다.
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