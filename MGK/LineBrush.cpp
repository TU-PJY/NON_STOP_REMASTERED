#include "LineBrush.h"
#include "TransformUtil.h"
#include "MathUtil.h"
#include "ImageUtil.h"
#include "ShaderUtil.h"
#include "CameraUtil.h"
#include "SystemResources.h"

void LineBrush::SetRenderType(int Opt) {
	RenderType = Opt;
}

void LineBrush::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void LineBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (GLfloat)R;
	Color.g = (1.0f / 255.0f) * (GLfloat)G;
	Color.b = (1.0f / 255.0f) * (GLfloat)B;
}

void LineBrush::Draw(GLfloat X1, GLfloat Y1, GLfloat X2, GLfloat Y2, GLfloat Width, GLfloat Transparency) {
	TranslateMatrix = glm::mat4(1.0f);
	ScaleMatrix = glm::mat4(1.0f);
	TransparencyValue = Transparency;

	Length = Math::CalcDistance(X1, Y1, X2, Y2);
	Rotation = Math::CalcRadians(X1, Y1, X2, Y2);

	Transform::Move(TranslateMatrix, X1, Y1);
	Transform::RotateRad(TranslateMatrix, Rotation);
	Transform::Move(TranslateMatrix, Length / 2.0, 0.0);
	Transform::Scale(ScaleMatrix, Length + Width, Width);

	Render();
}

void LineBrush::DrawLineX(GLfloat X1, GLfloat X2, GLfloat Y, GLfloat Width, GLfloat Transparency) {
	TranslateMatrix = glm::mat4(1.0f);
	ScaleMatrix = glm::mat4(1.0f);
	TransparencyValue = Transparency;

	Transform::Move(TranslateMatrix, (X1 + X2) / 2.0, Y);
	Transform::Scale(ScaleMatrix, fabs(X1 - X2) + Width, Width);

	Render();
}

void LineBrush::DrawLineY(GLfloat Y1, GLfloat Y2, GLfloat X, GLfloat Width, GLfloat Transparency) {
	TranslateMatrix = glm::mat4(1.0f);
	ScaleMatrix = glm::mat4(1.0f);
	TransparencyValue = Transparency;

	Transform::Move(TranslateMatrix, X, (Y1 + Y2) / 2.0);
	Transform::Scale(ScaleMatrix, Width, fabs(Y1 - Y2) + Width);

	Render();
}

void LineBrush::Render() {
	camera.SetCamera(RenderType);

	glUseProgram(ImageShader);
	camera.PrepareRender(SHADER_TYPE_IMAGE);

	glUniform1f(ImageTransparencyLocation, TransparencyValue);
	glUniform3f(ImageColorLocation, Color.r, Color.g, Color.b);
	glUniform1i(BoolBlurLocation, 0);
	glUniformMatrix4fv(ImageModelLocation, 1, GL_FALSE, value_ptr(TranslateMatrix * ScaleMatrix));

	imageUtil.Render(LineTex);
}
