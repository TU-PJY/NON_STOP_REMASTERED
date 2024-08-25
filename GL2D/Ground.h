#pragma once
#include "Framework.h"

class Ground : public GameObject {
private:
	Image Image{};
	
public:
	Ground();
	void Render();
};

