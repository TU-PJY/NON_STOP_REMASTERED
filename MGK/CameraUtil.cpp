#include "CameraUtil.h"

void Camera::CalculateASPECT() {
	ASPECT = (float)WIDTH / (float)HEIGHT;
	WindowRect.lx = -1.0 * ASPECT;
	WindowRect.ly = -1.0;
	WindowRect.rx = 1.0 * ASPECT;
	WindowRect.ry = 1.0;

	PREV_WIDTH = WIDTH;
	PREV_HEIGHT = HEIGHT;
}

void Camera::Init() {
	CalculateASPECT();

	ViewMatrix = glm::mat4(1.0f);
	Projection = glm::mat4(1.0f);
	TranslateMatrix = glm::mat4(1.0f);
	RotateMatrix = glm::mat4(1.0f);

	SetCamera(RENDER_TYPE_DEFAULT);
}

void Camera::SetCamera(int RenderType) {
	using namespace glm;

	if(PREV_WIDTH != WIDTH || PREV_HEIGHT != HEIGHT)
		CalculateASPECT();

	CamPos = vec3(0.0f, 0.0f, 1.0f);
	CamDirection = vec3(0.0f, 0.0f, 0.0f);
	CamUp = vec3(0.0f, 1.0f, 0.0f);

	if (RenderType == RENDER_TYPE_DEFAULT)
		StaticMode = false;
	else if (RenderType == RENDER_TYPE_STATIC)
		StaticMode = true;
}

void Camera::PrepareRender(int ShaderType) {
	ViewMatrix = glm::mat4(1.0f);
	Projection = glm::mat4(1.0f);

	if (StaticMode) {
		ViewMatrix = lookAt(CamPos, CamDirection, CamUp);
		Projection = glm::ortho((ASPECT * -1.0f), (ASPECT * 1.0f), -1.0f, 1.0f, -100.0f, 100.0f);
	}

	else {
		ViewMatrix = lookAt(CamPos, CamDirection, CamUp);
		ViewMatrix = ViewMatrix * TranslateMatrix * RotateMatrix;
		Projection = glm::ortho((ASPECT * -1.0f) / ZoomValue, (ASPECT * 1.0f) / ZoomValue, -1.0f / ZoomValue, 1.0f / ZoomValue, -100.0f, 100.0f);
	}

	if (ShaderType == SHADER_TYPE_IMAGE) {
		glUniformMatrix4fv(ImageProjectionLocation, 1, GL_FALSE, &Projection[0][0]);
		glUniformMatrix4fv(ImageViewLocation, 1, GL_FALSE, &ViewMatrix[0][0]);
		glUniform3f(ImageViewPositionLocation, CamPos.x, CamPos.y, CamPos.z);
	}

	else if (ShaderType == SHADER_TYPE_TEXT) {
		glUniformMatrix4fv(TextProjectionLocation, 1, GL_FALSE, &Projection[0][0]);
		glUniformMatrix4fv(TextViewLocation, 1, GL_FALSE, &ViewMatrix[0][0]);
		glUniform3f(TextViewPositionLocation, CamPos.x, CamPos.y, CamPos.z);
	}
}