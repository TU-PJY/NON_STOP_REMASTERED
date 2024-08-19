#include "CameraController.h"
#include "CameraUtil.h"
#include "Framework.h"
#include "MouseUtil.h"
#include <cmath>

GLfloat CameraController::GetPositionX() {
	return FinalPositionX;
}

GLfloat CameraController::GetHeight() {
	return FinalHeight;
}

CameraController::CameraController() {
	if (auto Player = framework.Find("player"); Player)
		Height = -(Player->GetHeight() + FLOOR_HEIGHT);
}

void CameraController::PushCamera(GLfloat Value) {

}

void CameraController::Update(float FT) {
	// 카메라는 플레이어를 추적
	if (auto Player = framework.Find("player"); Player) {
		PositionX = std::lerp(PositionX, -Player->GetPositionX(), FT * 5);
		Height = std::lerp(Height, -(Player->GetHeight() + FLOOR_HEIGHT), FT * 5);
	}

	if (auto Target = framework.Find("target"); Target) {
		PositionXByCursor = std::lerp(PositionXByCursor, Target->GetPositionX() * 0.3, FT * 10);
		HeightByCursor = std::lerp(HeightByCursor, Target->GetHeight() * 0.3, FT * 10);
	}

	FinalPositionX = PositionX - PositionXByCursor;
	FinalHeight = (FLOOR_HEIGHT + Height * 0.5) - HeightByCursor;

	camera.Move(FinalPositionX, FinalHeight);
}