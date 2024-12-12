#include "GameOverMode.h"
#include "Scene.h"
#include "MouseUtil.h"

class TextObject : public GameObject {
public:
	TextUtil text{};
	bool InitState{};

	void UpdateFunc(float FT) {
		if (!InitState) {
			text.Init(L"¸¼Àº °íµñ", FW_NORMAL);
			text.SetColor(1.0, 0.0, 0.0);
			text.SetAlign(ALIGN_MIDDLE);
			text.SetRenderType(RENDER_TYPE_STATIC);
			InitState = true;
		}
	}

	void RenderFunc() {
		text.Render(0.0, 0.0, 0.5, 1.0, L"GAME OVER");
	}
};

void GameOverMode::Start() {
	scene.AddObject(new TextObject, "text_object", LAYER_1);

	scene.RegisterController(Controller, MODE_TYPE_DEFAULT);
	scene.RegisterDestructor(Destructor);
	scene.RegisterModeName("GameOverMode");
}

void GameOverMode::Destructor() {

}

void GameOverMode::ProcessKeyboard(int State, unsigned char NormalKey, int SpecialKey) {
	if (State == NORMAL_KEY_DOWN) {
		switch (NormalKey) {
		case 27: //ESC
			scene.Exit();
			break;
		}
	}
}

void GameOverMode::ProcessMouseButton(int State) {
}

void GameOverMode::ProcessMouseWheel(int State) {
}



void GameOverMode::KeyDown(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(NORMAL_KEY_DOWN, KEY, NULL);
}

void GameOverMode::KeyUp(unsigned char KEY, int X, int Y) {
	ProcessKeyboard(NORMAL_KEY_UP, KEY, NULL);
}

void GameOverMode::SpecialKeyDown(int KEY, int X, int Y) {
	ProcessKeyboard(SPECIAL_KEY_DOWN, NULL, KEY);
}

void GameOverMode::SpecialKeyUp(int KEY, int X, int Y) {
	ProcessKeyboard(SPECIAL_KEY_UP, NULL, KEY);
}

void GameOverMode::MouseMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void GameOverMode::MousePassiveMotion(int X, int Y) {
	mouse.ConvertPosition(X, Y);
}

void GameOverMode::MouseWheel(int Button, int Wheel, int X, int Y) {
	if (Wheel > 0)
		ProcessMouseWheel(WHEEL_UP);
	else if (Wheel < 0)
		ProcessMouseWheel(WHEEL_DOWN);
}

void GameOverMode::MouseButton(int Button, int State, int X, int Y) {
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

void GameOverMode::Controller() {
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	glutMouseWheelFunc(MouseWheel);
	glutMouseFunc(MouseButton);
	glutSpecialFunc(SpecialKeyDown);
	glutSpecialUpFunc(SpecialKeyUp);
}

