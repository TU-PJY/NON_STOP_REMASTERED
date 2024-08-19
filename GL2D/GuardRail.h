#pragma once
#include "GameObject.h"

class GuardRail : public GameObject {
private:
	Image Image{};
	GLfloat PositionX{}, Height{};

public:
	GuardRail();
	void Update(float FT);
	void Render();
};

