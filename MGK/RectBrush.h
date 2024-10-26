#pragma once
#include "EngineHeader.h"

class LineRectBrush {
private:
	glm::mat4 TranslateMatrix{ 1.0f }, ScaleMatrix{ 1.0f };

	glm::vec3 Color{};
	GLfloat Length{};
	GLfloat TransparencyValue{};
	int RenderType{ RENDER_TYPE_DEFAULT };

	bool CamInheritanceCommand{};

public:
	LineRectBrush(bool Flag=false);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetColorRGB(int R, int G, int B);
	void SetRenderType(int Opt);
	void Draw(GLfloat X, GLfloat Y, GLfloat SizeX, GLfloat SizeY, GLfloat Width, GLfloat RotationValue, GLfloat Transparency = 1.0f);

private:
	void DrawLine(GLfloat X, GLfloat Y, GLfloat OffsetX, GLfloat OffsetY, GLfloat Width, GLfloat Height, GLfloat RotationValue);
	void Render();
};


class RectBrush {
private:
	glm::mat4 TranslateMatrix{ 1.0f }, ScaleMatrix{ 1.0f };

	glm::vec3 Color{};
	GLfloat Length{};
	GLfloat TransparencyValue{};
	int RenderType{ RENDER_TYPE_DEFAULT };

	bool CamInheritanceCommand{};

public:
	RectBrush(bool Flag=false);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetColorRGB(int R, int G, int B);
	void SetRenderType(int Opt);
	void Draw(GLfloat X, GLfloat Y, GLfloat SizeX, GLfloat SizeY, GLfloat RotationValue, GLfloat Transparency = 1.0f);

private:
	void Render();
};