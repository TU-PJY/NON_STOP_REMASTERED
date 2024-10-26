#pragma once
#include "EngineHeader.h"

class Camera {
private:
	glm::vec3 CamPos{}, CamDirection{}, CamUp{};
	bool StaticMode{};
	float FrameTime{};
	
public:
	glm::mat4 ViewMatrix{}, Projection{}, TranslateMatrix{}, RotateMatrix{};
	GLfloat ZoomValue{ 1.0f };

	void CalculateASPECT();
	void Init();
	void InputFrameTime(float FT);
	void SetCamera(int RenderType);
	void PrepareRender(int ShaderType);
};
extern Camera camera;