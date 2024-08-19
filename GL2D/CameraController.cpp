#include "CameraController.h"
#include "CameraUtil.h"
#include "Framework.h"
#include <cmath>

GLfloat CameraController::GetPosition() {
	return PositionX;
}

void CameraController::Update(float FT) {
	// 카메라는 플레이어를 추적
	if (auto Player = framework.Find("player"); Player) {
		PositionX = std::lerp(PositionX, -Player->GetPosition(), FT * 5);
		Height = std::lerp(Height, -(Player->GetHeight() + (1.0 + FLOOR_HEIGHT)) + 0.2, FT * 5);
	}

	camera.Move(PositionX, Height);
}