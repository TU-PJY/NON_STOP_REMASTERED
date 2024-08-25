#include "CameraController.h"
#include "CameraUtil.h"
#include "MouseUtil.h"
#include <cmath>
#include <iostream>

GLfloat CameraController::GetPositionX() {
	return FinalPositionX;
}

GLfloat CameraController::GetHeight() {
	return FinalHeight;
}

GLfloat CameraController::GetRotation() {
	return Rotation;
}

CameraController::CameraController() {
	if (auto Player = framework.Find("player"); Player)
		Height = -(Player->GetHeight() + FLOOR_HEIGHT);
}

void CameraController::PushCamera(GLfloat Value) {

}

void CameraController::ShakeCamera(GLfloat Value) {
	ShakeValue += Value;
}

void CameraController::AddTiltShake(GLfloat Value) {
	Rotation = Value;
}

void CameraController::Update(float FT) {
	// 카메라는 플레이어를 추적
	if (auto Player = framework.Find("player"); Player) {
		if(Player->GetPositionX() > 5.8)
			PositionX = std::lerp(PositionX, -5.8, FT * 5);
		else if(Player->GetPositionX() < -5.8)
			PositionX = std::lerp(PositionX, 5.8, FT * 5);
		else
			PositionX = std::lerp(PositionX, -Player->GetPositionX(), FT * 5);

		Height = std::lerp(Height, -(Player->GetHeight() + FLOOR_HEIGHT), FT * 5);
	}

	if (auto Target = framework.Find("target"); Target) {
		PositionXByCursor = std::lerp(PositionXByCursor, Target->GetPositionX() * 0.3, FT * 10);
		HeightByCursor = std::lerp(HeightByCursor, Target->GetHeight() * 0.3, FT * 10);
	}

	Rotation = std::lerp(Rotation, 0.0, FT * 10);

	ShakeX = randomUtil.Gen(Dist::Real, -ShakeValue, ShakeValue);
	ShakeY = randomUtil.Gen(Dist::Real, -ShakeValue, ShakeValue);

	ResultShakeX = std::lerp(ResultShakeX, ShakeX, FT * 5);
	ResultShakeY = std::lerp(ResultShakeY, ShakeY, FT * 5);

	ShakeValue = std::lerp(ShakeValue, 0.0, FT * 10);

	FinalPositionX = PositionX - PositionXByCursor - ResultShakeX;
	FinalHeight = (FLOOR_HEIGHT + Height * 0.5) - HeightByCursor - ResultShakeY;

	camera.Move(FinalPositionX, FinalHeight);
	camera.Rotate(Rotation);
}