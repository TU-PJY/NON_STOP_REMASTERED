#pragma once
#include "Framework.h"

class BulletAR : public GameObject {
private:
	Image Image{};
	GLfloat Rotation{};
	GLfloat BulletX{}, BulletY{};
	GLfloat MoveSpeed{};
	GLfloat MoveDistance{};
	int Dir{};

public:
	BulletAR(GLfloat RotationValue, GLfloat XPos, GLfloat YPos, GLfloat Speed, int DirValue);
	void Update(float FT);
	void Render();
};

