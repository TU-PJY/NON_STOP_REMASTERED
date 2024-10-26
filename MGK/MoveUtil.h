#pragma once
#include "EngineHeader.h"

namespace Move {
	void MoveStraight(GLfloat& Position, int MoveDirection, GLfloat Speed, float FT);
	void MoveStraight(GLfloat& Position, GLfloat Speed, float FT);
	void MoveForward(GLfloat& X, GLfloat& Y, GLfloat Speed, int MoveDirection, GLfloat RotationValue, float FT, bool Plus90Option);
	void MoveForward(GLfloat& X, GLfloat& Y, GLfloat Speed, GLfloat RotationValue, float FT, bool Plus90Option);
}