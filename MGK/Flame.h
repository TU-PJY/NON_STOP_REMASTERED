#pragma once
#include "GameObject.h"
#include "Scene.h"

class Flame : public GameObject {
private:
	GLfloat FlameTransparent{ 1.0f };
	glm::vec2 Position{};
	GLfloat Rotation{};
	TimerUtil timer;

public:
	Flame(GLfloat X, GLfloat Y, GLfloat RotationValue);
	void UpdateFunc(float FT);
	void RenderFunc();
};