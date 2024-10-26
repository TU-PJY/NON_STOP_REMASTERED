#pragma once
#include "EngineHeader.h"

class MouseUtil {
public:
	GLfloat x{}, y{};
	GLfloat CrossX{}, CrossY{};
	void ConvertPosition(int X, int Y);
};
extern MouseUtil mouse;