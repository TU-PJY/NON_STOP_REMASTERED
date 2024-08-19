#pragma once
#include "EngineHeader.h"

class MouseUtil {
public:
	GLfloat x{}, y{};
	void ConvertPosition(int X, int Y);
};
extern MouseUtil mouse;