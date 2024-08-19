#pragma once
#include "EngineHeader.h"

enum class ZoomOpt{ In, Out };

class Camera {
private:
	glm::vec3 CamPos{}, CamDirection{}, CamUp{};
	unsigned int ProjectionLocation{}, ViewLocation{}, ViewPosLocation{};
	
public:
	glm::mat4 ViewMatrix{}, Projection{};
	GLfloat Rotation{};
	GLfloat PositionX{}, PositionY{};
	GLfloat Zoom{ 1.0f };

	void CalculateASPECT();
	void Init();
	void SetCamera();
	void SetStaticCamera();
	void ProcessTransform(bool UseTextShader);

	void Move(GLfloat MoveX, GLfloat MoveY);
	void Rotate(GLfloat RotationValue);
	void SetZoom(ZoomOpt Type, GLfloat Value);
};
extern Camera camera;

GLfloat DivZoom(GLfloat Value);