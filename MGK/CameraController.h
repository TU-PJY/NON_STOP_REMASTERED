#pragma once
#include "EngineHeader.h"
#include "TimerUtil.h"

enum ZoomType
{ ZOOM_IN, ZOOM_OUT };

class CameraController {
public:
	glm::vec2 Position{};
	glm::vec2 Shake{};
	glm::vec2 ShakeValue{};
	GLfloat Rotation{};

	TimerUtil timer;

	void Update(float FT);
	void ShakeCamera(float FT);
	void AddShakeValue(GLfloat Value);
	void InitMatrix();
	void CalcMatrix();
	void Move(GLfloat X, GLfloat Y);
	void Move(glm::vec2 PositionValue);
	void Rotate(GLfloat Value);
	void Zoom(int Type, GLfloat Value);
	void ChangeZoom(GLfloat Value);
	GLfloat CalculateNextZoom(int Type, GLfloat Value);
};
extern CameraController cameraCon;