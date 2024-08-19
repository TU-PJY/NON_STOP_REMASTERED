#pragma once
#include "GameObject.h"

class CameraController : public GameObject {
private:
	GLfloat PositionX{}, Height{};

public:
	GLfloat GetPosition();
	void Update(float FT);
};

