#pragma once
#include "GameObject.h"

class CameraController : public GameObject {
private:
	GLfloat PositionX{}, Height{};
	GLfloat PositionXByCursor{}, HeightByCursor{};

	GLfloat FinalPositionX{};
	GLfloat FinalHeight{};

public:
	CameraController();
	void PushCamera(GLfloat Value);
	GLfloat GetPositionX();
	GLfloat GetHeight();
	void Update(float FT);
};

