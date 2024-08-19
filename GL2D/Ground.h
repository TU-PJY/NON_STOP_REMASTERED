#pragma once
#include "GameObject.h"

class Ground : public GameObject {
private:
	Image Image{};
	
public:
	Ground();
	void Render();
};

