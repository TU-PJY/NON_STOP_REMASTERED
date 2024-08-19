#pragma once
#include "GameObject.h"

class CameraController : public GameObject {
private:
	GLfloat PositionX{}, Height{};
	GLfloat PositionByCursor{}, HeightByCursor{};

public:
	CameraController();
	GLfloat GetPosition();
	GLfloat GetHeight();
	void Update(float FT);
};

