#pragma once
#include "EngineHeader.h"

class LineBrush {
private:
	glm::mat4 TranslateMatrix{ 1.0f }, ScaleMatrix{ 1.0f };

	GLfloat Width{};
	GLfloat Rotation;
	GLfloat Length{};
	glm::vec3 Color{};
	GLfloat TransparencyValue{};

	int RenderType { RENDER_TYPE_DEFAULT };

public:
	void SetRenderType(int Opt);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetColorRGB(int R, int G, int B);
	void Draw(GLfloat X1, GLfloat Y1, GLfloat X2, GLfloat Y2, GLfloat Width, GLfloat Transparency = 1.0);
	void DrawLineX(GLfloat X1, GLfloat X2, GLfloat Y, GLfloat Width, GLfloat Transparency = 1.0);
	void DrawLineY(GLfloat Y1, GLfloat Y2, GLfloat X, GLfloat Width, GLfloat Transparency = 1.0);

private:
	void Render();
};