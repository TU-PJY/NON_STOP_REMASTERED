#pragma once
#include "Framework.h"

class Target : public GameObject {
private:
	Image TargetImage{};
	GLfloat PositionX{}, PositionY{};

	GLfloat PrevMouseX{}, PrevMouseY{};
	GLfloat CurrentMouseX{}, CurrentMouseY{};

	GLfloat ViewportX{}, ViewportY{};

public:
	GLfloat GetPositionX();
	GLfloat GetHeight();
	Target();
	void Update(float FT);
	void Render();
};

