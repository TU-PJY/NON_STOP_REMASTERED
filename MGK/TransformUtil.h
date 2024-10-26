#pragma once
#include "EngineHeader.h"

namespace Transform {
	void Move(glm::mat4& Matrix, GLfloat X, GLfloat Y);
	void Move(glm::mat4& Matrix, glm::vec2 Position);
	void Rotate(glm::mat4& Matrix, GLfloat Degree);
	void RotateRad(glm::mat4& Matrix, GLfloat Radians);
	void Scale(glm::mat4& Matrix, GLfloat X, GLfloat Y);
	void Scale(glm::mat4& Matrix, glm::vec2 Size);
	void RotateV(glm::mat4& Matrix, GLfloat Degree);
	void RotateH(glm::mat4& Matrix, GLfloat Degree);
}