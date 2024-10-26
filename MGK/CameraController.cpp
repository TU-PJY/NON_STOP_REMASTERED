#include "CameraController.h"
#include "CameraUtil.h"
#include "TransformUtil.h"

void CameraController::Update(float FT) {
	// add logic here

	CalcMatrix();
}

void CameraController::InitMatrix() {
	camera.TranslateMatrix = glm::mat4(1.0f);
	camera.RotateMatrix = glm::mat4(1.0f);
}

void CameraController::CalcMatrix() {
	InitMatrix();
	Transform::Move(camera.TranslateMatrix, Position.x, Position.y);
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