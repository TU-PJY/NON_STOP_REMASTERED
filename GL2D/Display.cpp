#include "EngineHeader.h"

GLvoid DisplayReshape(int w, int h) {
	glViewport(0, 0, w, h);
	WIDTH = w;
	HEIGHT = h;
}

void SetBackColor(GLfloat R, GLfloat G, GLfloat B) {
	BackColor.r = R;
	BackColor.g = G;
	BackColor.b = B;
}