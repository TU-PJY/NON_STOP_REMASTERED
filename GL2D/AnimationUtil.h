#pragma once
#include "EngineHeader.h"
#include <cmath>

class LoopSinAnimation {
private:
	GLfloat Num{};

public:
	GLfloat Update(GLfloat MoveScale, float FrameTime);
	void Reset();
};


class PopBounceAnimation {
private:
	GLfloat Num1{}, Num2{}, Num3{};

public:
	GLfloat Update(GLfloat SizeDest, GLfloat ShakeValue, GLfloat SizeIncreaseSpeed, GLfloat ShakeSpeed, GLfloat ShakeReduceSpeed, float FrameTime);
	void Reset();
};


class LerpSinAnimation {
private:
	GLfloat Num{};
	GLfloat Acc{};
	GLfloat Result{};
	bool Increase{ true };

public:
	GLfloat Update(GLfloat From, GLfloat To, GLfloat Speed, GLfloat AccValue, float FrameTime);
	void Reset();
};