#pragma once
#include "Framework.h"

class Casing : public GameObject {
private:
	Image Image{};
	GLfloat Rotation{}, RotationSpeed{4000};
	GLfloat CasingPositionX{}, CasingPositionY{};
	PhysicsUtil psUtil;
	GLfloat FallingAcc{};
	GLfloat MoveSpeed{};

	int Dir{};
	bool MoveActivated{ true };

	RandomUtil randomUtil;
	TimerUtil timerUtil;

public:
	Casing(GLfloat XPos, GLfloat YPos, int Dir);
	void Update(float FT);
	void Render();
};

