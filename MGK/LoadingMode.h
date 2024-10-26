#pragma once
#include "EngineHeader.h"

namespace LoadingMode {
	void Start();
	void Destructor();

	void ProcessKeyboard(int State, unsigned char NormalKey, int SpecialKey);
	void ProcessMouseButton(int State);
	void ProcessMouseWheel(int Button);
	void KeyDown(unsigned char KEY, int X, int Y);
	void KeyUp(unsigned char KEY, int X, int Y);
	void SpecialKeyUp(int KEY, int X, int Y);
	void SpecialKeyDown(int KEY, int X, int Y);
	void MouseMotion(int X, int Y);
	void MousePassiveMotion(int X, int Y);
	void MouseButton(int Button, int State, int X, int Y);
	void MouseWheel(int Button, int Wheel, int X, int Y);
	void Controller();
}