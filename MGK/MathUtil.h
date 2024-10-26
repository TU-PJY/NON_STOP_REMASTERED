#pragma once
#include "EngineHeader.h"

namespace Math {
	void LookAt(GLfloat& RotationVar, GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY, GLfloat RotationSpeed, float FT);
	void LookAt(GLfloat& RotationVar, GLfloat Rotation, GLfloat RotationSpeed, float FT);
	GLfloat CalcDistance(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY);
	GLfloat Lerp(GLfloat Value, GLfloat Dest, GLfloat Speed, float FrameTime);
	GLfloat CalcDegree(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY);
	GLfloat CalcRadians(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY);
	GLfloat NormalizeDegree(GLfloat Degree);
	GLfloat CalculateShortestRotation(GLfloat CurrentDegree, GLfloat DegreeDest);
}