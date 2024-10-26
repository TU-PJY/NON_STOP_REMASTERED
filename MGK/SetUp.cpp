#include "EngineHeader.h"
#include "Scene.h"
#include "CameraUtil.h"
#include "CameraController.h"
#include "ImageUtil.h"
#include "FontUtil.h"
#include "ShaderUtil.h"
#include "SoundUtil.h"
#include "MouseUtil.h"
#include "Setting.h"
#include <iostream>
#include <windows.h>

#include "LoadingMode.h"


int WIDTH = WINDOW_WIDTH;
int HEIGHT = WINDOW_HEIGHT;
int PREV_WIDTH, PREV_HEIGHT;

float DestFPS;
int FPSLimit;

bool FullscreenState;

GLfloat ASPECT;
ViewportRect WindowRect;
glm::vec3 BackColor;

ShaderUtil shaderUtil;
Camera camera;
CameraController cameraCon;
ImageUtil imageUtil;
SoundUtil soundUtil;
MouseUtil mouse;
FontUtil fontUtil;
Scene scene;

void Framework::SetupSystem(int argc, char** argv) {
	HWND hWnd = GetConsoleWindow();

	if (!SHOW_CONSOLE)
		ShowWindow(hWnd, SW_HIDE);
	else
		ShowWindow(hWnd, SW_SHOWNORMAL);

	glutInit(&argc, argv);
	SetupWindow();
	LoadShader();
	SetGlOption();
	InitSystem();
}


void Framework::SetupWindow() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GL_MULTISAMPLE);

	glutInitWindowPosition(GetSystemMetrics(SM_CXSCREEN) / 2 - WIDTH / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - HEIGHT / 2);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow(WINDOW_NAME);

	if (FULL_SCREEN_OPTION) {
		glutFullScreen();
		WIDTH = GetSystemMetrics(SM_CXSCREEN);
		HEIGHT = GetSystemMetrics(SM_CYSCREEN);
	}

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Unable to initialize GLEW\n\n";
		exit(EXIT_FAILURE);
	}
}

void Framework::LoadShader() {
	shaderUtil.LoadVertexShader("MGKResource//GLSL//Vertex.glsl");
	shaderUtil.LoadFragmentShader("MGKResource//GLSL//Fragment_Image.glsl");
	shaderUtil.CreateShader(ImageShader);

	shaderUtil.LoadFragmentShader("MGKResource//GLSL//Fragment_Text.glsl");
	shaderUtil.CreateShader(TextShader);
	shaderUtil.CreateShaderLocation();
}

void Framework::SetGlOption() {
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Framework::InitSystem() {
	FPSLimit = FRAME_LIMITS;
	if (FPSLimit > 0)
		DestFPS = 1000.0 / (float)FPSLimit;

	scene.Init(LoadingMode::Start);
}