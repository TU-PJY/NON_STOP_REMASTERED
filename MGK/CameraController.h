#pragma once
#include "EngineHeader.h"

enum ZoomType
{ ZOOM_IN, ZOOM_OUT };

class CameraController {
public:
	glm::vec2 Position{};
	GLfloat Rotation{};

	void Update(float FT);
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