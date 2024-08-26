#pragma once
#include "Framework.h"

class Target : public GameObject {
private:
	Image TargetImage{};
	GLfloat PositionX{}, PositionY{};

	GLfloat PrevMouseX{}, PrevMouseY{};
	GLfloat CurrentMouseX{}, CurrentMouseY{};

	RandomUtil randomUtil;

public:
	GLfloat GetPositionX();
	GLfloat GetHeight();
	Target();
	void GiveRecoil(GLfloat Value);
	void Update(float FT);
	void Render();
};

