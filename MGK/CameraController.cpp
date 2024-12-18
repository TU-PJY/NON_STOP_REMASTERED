#include "CameraController.h"
#include "CameraUtil.h"
#include "TransformUtil.h"
#include "Scene.h"
#include "RandomUtil.h"
#include "MathUtil.h"
#include "EXUtil.h"

void CameraController::Update(float FT) {
	// playmode 카메라 무빙 업데이트
	if (scene.Mode() == "PlayMode") {
		// 플레이어의 위치를 추적한다.
		if (auto player = scene.Find("player"); player)
			Position.x = -player->GetPosition().x;

		// 맵 가장자리에 카메라가 위치할 경우 추적하지 않는다.
		if (Position.x + ASPECT > 7.5)
			Position.x = 7.5 - ASPECT;

		else if (Position.x - ASPECT < -7.5)
			Position.x = -7.5 + ASPECT;

		ShakeCamera(FT);
		UpdatePush(FT);
	}
	
	CalcMatrix();
}

void CameraController::ShakeCamera(float FT) {
	// 흔들림 정도 랜덤 생성
	GLfloat RandShakeX = Random::Gen(DIST_REAL, -ShakeValue.x, ShakeValue.x);
	GLfloat RandShakeY = Random::Gen(DIST_REAL, -ShakeValue.y, ShakeValue.y);

	// 부드럽게 화면을 흔든다.
	Shake.x = Math::Lerp(Shake.x, RandShakeX, 1, FT);
	Shake.y = Math::Lerp(Shake.y, RandShakeY, 1, FT);

	// 흔들림 수치가 감소한다.
	ShakeValue.x = Math::Lerp(ShakeValue.x, 0.0, 0.5, FT);
	ShakeValue.y = Math::Lerp(ShakeValue.y, 0.0, 0.5, FT);

	// 흔들림 수치가 0.0미만으로 감소되지 않도록 한다.
	EX::ClampValue(ShakeValue.x, 0.0, CLAMP_LESS);
	EX::ClampValue(ShakeValue.y, 0.0, CLAMP_LESS);
}

void CameraController::UpdatePush(float FT) {
	PushValue = Math::Lerp(PushValue, 0.0, 0.4, FT);
}

void CameraController::AddShakeValue(GLfloat Value) {
	ShakeValue.x += Value;
	ShakeValue.y += Value;
}

void CameraController::Push(GLfloat Value) {
	PushValue = Value;
}

void CameraController::InitMatrix() {
	camera.TranslateMatrix = glm::mat4(1.0f);
	camera.RotateMatrix = glm::mat4(1.0f);
}

void CameraController::CalcMatrix() {
	InitMatrix();
	Transform::Move(camera.TranslateMatrix, Position.x + Shake.x, Position.y + Shake.y - PushValue);
	Transform::Rotate(camera.RotateMatrix, Rotation);
}

void CameraController::Move(GLfloat X, GLfloat Y) {
	Position.x = X;
	Position.y = Y;
}

void CameraController::Move(glm::vec2 PositionValue) {
	Position = PositionValue;
}

void CameraController::Rotate(GLfloat Value) {
	Rotation = Value;
}

void CameraController::Zoom(int Type, GLfloat Value) {
	if (Type == ZOOM_IN)
		camera.ZoomValue = camera.ZoomValue / (1.0f - Value);
	else if (Type == ZOOM_OUT)
		camera.ZoomValue = camera.ZoomValue * (1.0f - Value);
}

void CameraController::ChangeZoom(GLfloat Value) {
	camera.ZoomValue = Value;
}

GLfloat CameraController::CalculateNextZoom(int Type, GLfloat Value) {
	if(Type == ZOOM_IN)
		return camera.ZoomValue / (1.0f - Value);
	else if(Type == ZOOM_OUT)
		return camera.ZoomValue * (1.0f - Value);
}