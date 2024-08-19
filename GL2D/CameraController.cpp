#include "CameraController.h"
#include "CameraUtil.h"
#include "Framework.h"
#include <cmath>

GLfloat CameraController::GetPosition() {
	return PositionX;
}

GLfloat CameraController::GetHeight() {
	return Height;
}

CameraController::CameraController() {
	if (auto Player = framework.Find("player"); Player)
		Height = -(Player->GetHeight() + FLOOR_HEIGHT);
}

void CameraController::Update(float FT) {
	// 카메라는 플레이어를 추적
	if (auto Player = framework.Find("player"); Player) {
		PositionX = std::lerp(PositionX, -Player->GetPosition(), FT * 5);
		Height = std::lerp(Height, -(Player->GetHeight() + FLOOR_HEIGHT), FT * 5);
	}

	camera.Move(PositionX, FLOOR_HEIGHT + Height * 0.5);
}