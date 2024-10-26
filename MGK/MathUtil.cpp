#include "MathUtil.h"
#include <cmath>


void Math::LookAt(GLfloat& RotationVar, GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY, GLfloat RotationSpeed, float FT) {
	GLfloat TargetAngle{}, ShortestAngle{};
	TargetAngle = atan2(ToY - FromY, ToX - FromX) * (180 / 3.1415) - 90.0;
	TargetAngle = NormalizeDegree(TargetAngle);

	if (RotationSpeed > 0)
		ShortestAngle = std::lerp(ShortestAngle, CalculateShortestRotation(RotationVar, TargetAngle), FT * RotationSpeed);
	else
		ShortestAngle = CalculateShortestRotation(RotationVar, TargetAngle);

	RotationVar = NormalizeDegree(RotationVar + ShortestAngle);
}

void Math::LookAt(GLfloat& RotationVar, GLfloat Rotation, GLfloat RotationSpeed, float FT) {
	GLfloat TargetAngle{}, ShortestAngle{};
	TargetAngle = NormalizeDegree(Rotation);

	if (RotationSpeed > 0)
		ShortestAngle = std::lerp(ShortestAngle, CalculateShortestRotation(RotationVar, TargetAngle), FT * RotationSpeed);
	else
		ShortestAngle = CalculateShortestRotation(RotationVar, TargetAngle);

	RotationVar = NormalizeDegree(RotationVar + ShortestAngle);
}

GLfloat Math::Lerp(GLfloat Value, GLfloat Dest, GLfloat Speed, float FrameTime) {
	GLfloat Result = Speed * FrameTime;
	if (Result <= 1.0 && Result >= 0.0)
		return std::lerp(Value, Dest, Result);
	else
		return Value;
}

GLfloat Math::CalcDistance(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY) {
	return  std::sqrt(std::pow(FromX - ToX, 2) + std::pow(FromY - ToY, 2));
}

GLfloat Math::CalcDegree(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY) {
	return atan2(ToY - FromY, ToX - FromX) * (180.0 / 3.1415);
}

GLfloat Math::CalcRadians(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY) {
	return atan2(ToY - FromY, ToX - FromX);
}



//////////////////////////////////



GLfloat Math::NormalizeDegree(GLfloat Degree) {
	while (Degree < 0) Degree += 360;
	while (Degree >= 360) Degree -= 360;
	return Degree;
}

GLfloat Math::CalculateShortestRotation(GLfloat CurrentDegree, GLfloat DegreeDest) {
	GLfloat Diff = DegreeDest - CurrentDegree;

	if (Diff > 180)
		Diff -= 360;
	else if (Diff < -180)
		Diff += 360;

	return Diff;
}