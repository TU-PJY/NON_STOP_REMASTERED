#pragma once
#include "Framework.h"

class CameraController : public GameObject {
private:
	GLfloat PositionX{}, Height{};
	GLfloat PositionXByCursor{}, HeightByCursor{};

	GLfloat FinalPositionX{};
	GLfloat FinalHeight{};

	GLfloat Rotation{};
	GLfloat TiltShake{};
	GLfloat ShakeValue{}, ShakeX{}, ShakeY{};
	GLfloat ResultShakeX{}, ResultShakeY{};

	RandomUtil randomUtil;

public:
	CameraController();
	void PushCamera(GLfloat Value);
	void ShakeCamera(GLfloat Value);
	void AddTiltShake(GLfloat Value);
	GLfloat GetPositionX();
	GLfloat GetHeight();
	GLfloat GetRotation();
	void Update(float FT);
};

