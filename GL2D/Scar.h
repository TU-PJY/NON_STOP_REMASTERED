#pragma once
#include "Framework.h"

class Scar : public GameObject {
private:
	Image Image{};
	GLfloat Rotation{};

	GLfloat GunX{}, GunY{};
	GLfloat ViewportX{}, ViewportY{};

public:
	Scar();
	void Update(float FT);
	void Render();
};

