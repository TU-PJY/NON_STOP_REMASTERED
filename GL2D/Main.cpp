#include "EngineHeader.h"
#include "Framework.h"
#include "CameraUtil.h"
#include "SoundUtil.h"

// type start mode here
#include "Mode1.h"
Start_Mode StartMode = Mode1::GameMode1;

// frametime values
float PrevTime, CurrentTime, DeltaTime;

GLvoid GLMain() {
	glClearColor(BackColor.r, BackColor.g, BackColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	framework.Routine();
	soundUtil.Update();

	CurrentTime = float(glutGet(GLUT_ELAPSED_TIME));
	DeltaTime = (CurrentTime - PrevTime) / 1000.0;

	framework.InputFrameTime(DeltaTime);

	PrevTime = CurrentTime;

	glutSwapBuffers();
	glutPostRedisplay();
}

void main(int argc, char** argv) {
	SetupSystem(argc, argv);
	glutDisplayFunc(GLMain);
	glutReshapeFunc(DisplayReshape);
	glutMainLoop();
}