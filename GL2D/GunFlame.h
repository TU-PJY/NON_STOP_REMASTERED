#pragma once
#include "Framework.h"

class GunFlame : public GameObject {
private:
	Image Image{}, Blur{};
	GLfloat FlamePositionX{}, FlamePositionY{};
	GLfloat Rotation{};
	
	TimerUtil timerUtil;

	GameObject* GunPtr{};

public:
	GunFlame(GLfloat RotationValue, GLfloat XPos, GLfloat YPos);
	void Update(float FT);
	void Render();
};

