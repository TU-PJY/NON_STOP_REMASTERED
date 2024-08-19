#pragma once
#include "GameObject.h"

class BackGround : public GameObject {
private:
	Image Image{};
	GLfloat PositionX{}, Height{};

public:
	BackGround();
	void Update(float FT);
	void Render();
};