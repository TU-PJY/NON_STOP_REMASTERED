#pragma once
#include "EngineHeader.h"

enum Edge { 
	EDGE_LEFT_DOWN, 
	EDGE_LEFT_UP, 
	EDGE_RIGHT_UP,
	EDGE_RIGHT_DOWN 
};

namespace UI {
	void ClampMatrixToCorner(int EdgePosition, glm::mat4& Matrix, GLfloat Width, GLfloat Height, GLfloat GapX, GLfloat GapY);
	void ClampMatrixToPosition(glm::mat4& Matrix, GLfloat PositionX, GLfloat PositionY, GLfloat Width, GLfloat Height, GLfloat GapX, GLfloat GapY);
	void ClampPositionToCorner(int EdgePosition, GLfloat& PositionX, GLfloat& PositionY, GLfloat Width, GLfloat Height, GLfloat GapX, GLfloat GapY);
	void ClampPositionToCoord(GLfloat& ThisPositionX, GLfloat& ThisPositionY, GLfloat OtherPositionX, GLfloat OtherPositionY, GLfloat Width, GLfloat Height, GLfloat GapX, GLfloat GapY);
}