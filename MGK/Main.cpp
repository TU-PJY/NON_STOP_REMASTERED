#include "EngineHeader.h"
#include "CameraUtil.h"
#include "CameraController.h"
#include "Scene.h"
#include "SoundUtil.h"
#include <thread>
#include <chrono>

bool UpdateActivateCommand;

// frametime values
float PrevTime, CurrentTime, DeltaTime;

GLvoid Framework::Framework() {
	glClearColor(BackColor.r, BackColor.g, BackColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	if (UpdateActivateCommand) {
		cameraCon.Update(DeltaTime);
		scene.Routine();
		soundUtil.Update();
	}

	CurrentTime = float(glutGet(GLUT_ELAPSED_TIME));
	
	if(FPSLimit > 0)
		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast <int>(DestFPS)));

	DeltaTime = (CurrentTime - PrevTime) / 1000.0;
	scene.InputFrameTime(DeltaTime);

	PrevTime = CurrentTime;

	glutSwapBuffers();
	glutPostRedisplay();

	UpdateActivateCommand = true;
}

void main(int argc, char** argv) {
	Framework::SetupSystem(argc, argv);
	glutDisplayFunc(Framework::Framework);
	glutReshapeFunc(Framework::DisplayReshape);
	glutMainLoop();
}