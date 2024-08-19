#pragma once
#include "MouseUtil.h"
#include <string>

namespace Loading_Mode {
	std::string LoadingMode();

	void ProcessKeyboard(KeyType Type, KeyState State, unsigned char NormalKey, int SpecialKey);
	void ProcessMouse(int Button, int State, int X, int Y);
	void ProcessMouseWheel(int Button, int Wheel, int X, int Y);
	void KeyDown(unsigned char KEY, int X, int Y);
	void KeyUp(unsigned char KEY, int X, int Y);
	void SpecialKeyUp(int KEY, int X, int Y);
	void SpecialKeyDown(int KEY, int X, int Y);
	void MouseMotion(int X, int Y);
	void MousePassiveMotion(int X, int Y);
	void Controller();
}