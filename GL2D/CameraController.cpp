#include "CameraController.h"
#include "CameraUtil.h"
#include "Framework.h"
#include "MouseUtil.h"
#include <cmath>

GLfloat CameraController::GetPosition() {
	return PositionX - PositionByCursor;
}

GLfloat CameraController::GetHeight() {
	return (FLOOR_HEIGHT + Height * 0.5) - HeightByCursor;
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

	PositionByCursor = std::lerp(PositionByCursor, mouse.x * 0.3, FT * 10);
	HeightByCursor = std::lerp(HeightByCursor, mouse.y * 0.3, FT * 10);

	camera.Move(PositionX - PositionByCursor, (FLOOR_HEIGHT + Height * 0.5) - HeightByCursor);
}