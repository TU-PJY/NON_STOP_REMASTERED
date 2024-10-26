#pragma once
#pragma warning(disable: 4244) 
#pragma warning(disable: 4305) 
#include "Setting.h"
#include "glew.h"
#include "freeglut.h"
#include "freeglut_ext.h"
#include "glm/ext.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "fmod.hpp"
#include "fmod_errors.h"
#include <iostream>

extern float DestFPS;
extern int FPSLimit;

extern bool FullscreenState;

typedef void(*StartModePtr)(void);
using Sound = FMOD::Sound*;
using SoundChannel = FMOD::Channel*;
using ShaderLocation = unsigned int;

enum KeyState { 
	NORMAL_KEY_DOWN, 
	NORMAL_KEY_UP, 
	SPECIAL_KEY_DOWN, 
	SPECIAL_KEY_UP 
};

enum WheelState { 
	WHEEL_DOWN, 
	WHEEL_UP 
};

enum ButtonState { 
	LEFT_BUTTON_DOWN, 
	LEFT_BUTTON_UP, 
	RIGHT_BUTTON_DOWN, 
	RIGHT_BUTTON_UP, 
	MIDDLE_BUTTON_DOWN, 
	MIDDLE_BUTTON_UP 
};

enum SpecialKeySet {
	SK_ARROW_UP =    GLUT_KEY_UP,
	SK_ARROW_DOWN =  GLUT_KEY_DOWN,
	SK_ARROW_RIGHT = GLUT_KEY_RIGHT,
	SK_ARROW_LEFT =  GLUT_KEY_LEFT,
	SK_PAGE_UP =     GLUT_KEY_PAGE_UP,
	SK_PAGE_DOWN =   GLUT_KEY_PAGE_DOWN,
	SK_HOME =		 GLUT_KEY_HOME,
	SK_END =		 GLUT_KEY_END,
	SK_INSERT =		 GLUT_KEY_INSERT,
	SK_DELETE =      GLUT_KEY_DELETE,
	SK_F1 =			 GLUT_KEY_F1,
	SK_F2 =			 GLUT_KEY_F2,
	SK_F3 =			 GLUT_KEY_F3,
	SK_F4 =			 GLUT_KEY_F4,
	SK_F5 =			 GLUT_KEY_F5,
	SK_F6 =			 GLUT_KEY_F6,
	SK_F7 =			 GLUT_KEY_F7,
	SK_F8 =			 GLUT_KEY_F8,
	SK_F9 =			 GLUT_KEY_F9,
	SK_F10 =		 GLUT_KEY_F10,
	SK_F11 =		 GLUT_KEY_F11,
	SK_F12 =		 GLUT_KEY_F12,
	SK_SHIFT_RIGHT = GLUT_KEY_SHIFT_R,
	SK_SHIFT_LEFT =  GLUT_KEY_SHIFT_L,
	SK_CTRL_RIGHT =  GLUT_KEY_CTRL_R,
	SK_CTRL_LEFT =   GLUT_KEY_CTRL_L,
	SK_ALT_RIGHT =   GLUT_KEY_ALT_R,
	SK_ALT_LEFT =    GLUT_KEY_ALT_L
};

enum NormalKeySet {
	NK_SPACE =  32,
	NK_ENTER =  13,
	NK_ESCAPE = 27,
	NK_TAB =    9
};

enum RenderType {
	RENDER_TYPE_DEFAULT, 
	RENDER_TYPE_STATIC 
};

enum ShaderType { 
	SHADER_TYPE_IMAGE, 
	SHADER_TYPE_TEXT 
};

// corner position of display
struct ViewportRect {
	GLfloat lx, ly, rx, ry;
};
extern ViewportRect WindowRect;

// image struct
typedef struct {
	unsigned int Texture;
	int Width, Height;
}Image;

// display width, height and apsect ratio
extern int WIDTH, HEIGHT;
extern int PREV_WIDTH, PREV_HEIGHT;
extern GLfloat ASPECT;

extern glm::vec3 BackColor;
extern StartModePtr StartMode;

// global scope shader
extern GLuint ImageShader;
extern GLuint TextShader;

// global shader location
extern ShaderLocation ImageTransparencyLocation, ImageColorLocation, ImageModelLocation;
extern ShaderLocation BlurStrengthLocation, BoolBlurLocation, TexelSizeLocation;
extern ShaderLocation TextTransparencyLocation, TextColorLocation, TextModelLocation;
extern ShaderLocation ImageProjectionLocation, ImageViewLocation, ImageViewPositionLocation;
extern ShaderLocation TextProjectionLocation, TextViewLocation, TextViewPositionLocation;

namespace Framework {
	GLvoid Framework();
	GLvoid DisplayReshape(int w, int h);
	void SwitchScreenState();
	void SetFrameLimit(int FrameLimit);
	void SetBackColor(GLfloat R, GLfloat G, GLfloat B);
	void SetBackColorRGB(int R, int G, int B);
	void SetupSystem(int argc, char** argv);
	void InitSystem();
	void SetGlOption();
	void SetupWindow();
	void LoadShader();
}