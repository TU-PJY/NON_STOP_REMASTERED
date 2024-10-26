#include "CollisionUtil.h"
#include "TransformUtil.h"
#include "ImageUtil.h"
#include "CameraUtil.h"
#include "SystemResources.h"


void AABB::Update(GLfloat X, GLfloat Y, GLfloat xScale, GLfloat yScale) {
	OffsetX = xScale / 2;
	OffsetY = yScale / 2;

	LeftX = X - OffsetX;
	RightX = X + OffsetX;
	LeftY = Y - OffsetY;
	RightY = Y + OffsetY;

	Center.x = X;
	Center.y = Y;

	Width = xScale;
	Height = yScale;
}

void AABB::Render() {
#ifdef SHOW_BOUND_BOX
	LineRect.SetColor(1.0, 0.0, 0.0);
	Rect.SetColor(1.0, 0.0, 0.0);

	LineRect.Draw(Center.x, Center.y, Width, Height, 0.01, 0.0);

	if (Collide)
		Rect.Draw(Center.x, Center.y, Width, Height, 0.0, 0.3);
#endif
}

bool AABB::CheckCollision(const AABB& Other) {
	if (RightX < Other.LeftX || LeftX > Other.RightX) {
		Collide = false;
		return false;
	}

	if (RightY < Other.LeftY || LeftY > Other.RightY) {
		Collide = false;
		return false;
	}
	
	Collide = true;
	return true;
}

bool AABB::CheckCollisionPoint(GLfloat X, GLfloat Y) {
	if ((LeftX <= X && X <= RightX) && (LeftY <= Y && Y <= RightY)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}



void OOBB::Update(GLfloat X, GLfloat Y, GLfloat BoxWidth, GLfloat BoxHeight, GLfloat RotationValue) {
	Center = glm::vec2(X, Y);
	Offset = glm::vec2(BoxWidth / 2, BoxHeight / 2);

	GLfloat Rad = glm::radians(RotationValue);
	Axis[0] = glm::vec2(std::cos(Rad), std::sin(Rad));
	Axis[1] = glm::vec2(-std::sin(Rad), std::cos(Rad));

	Width = BoxWidth;
	Height = BoxHeight;
	Rotation = RotationValue;
}

void OOBB::Render() {
#ifdef SHOW_BOUND_BOX
	LineRect.SetColor(1.0, 0.0, 0.0);
	Rect.SetColor(1.0, 0.0, 0.0);

	LineRect.Draw(Center.x, Center.y, Width, Height, 0.01, Rotation);

	if (Collide)
		Rect.Draw(Center.x, Center.y, Width, Height, Rotation, 0.3);
#endif
}

std::pair<float, float> OOBB::Project(const OOBB& oobb, const glm::vec2& Axis) {
	Corners[0] = oobb.Center + oobb.Axis[0] * oobb.Offset.x + oobb.Axis[1] * oobb.Offset.y;
	Corners[1] = oobb.Center + oobb.Axis[0] * oobb.Offset.x - oobb.Axis[1] * oobb.Offset.y;
	Corners[2] = oobb.Center - oobb.Axis[0] * oobb.Offset.x + oobb.Axis[1] * oobb.Offset.y;
	Corners[3] = oobb.Center - oobb.Axis[0] * oobb.Offset.x - oobb.Axis[1] * oobb.Offset.y;

	float Min = glm::dot(Corners[0], Axis);
	float Max = Min;

	for (int i = 1; i < 4; i++) {
		float Projection = glm::dot(Corners[i], Axis);

		if (Projection < Min)
			Min = Projection;

		if (Projection > Max)
			Max = Projection;
	}

	return std::make_pair(Min, Max);
}

bool OOBB::OverlapOnAxis(const OOBB& oobb1, const OOBB& oobb2, const glm::vec2& Axis) {
	auto [Min1, Max1] = Project(oobb1, Axis);
	auto [Min2, Max2] = Project(oobb2, Axis);

	return !(Max1 < Min2 || Max2 < Min1);
}

bool OOBB::CheckCollision(const OOBB& Other) {
	glm::vec2 Axises[] = { Axis[0], Axis[1], Other.Axis[0], Other.Axis[1] };

	for (const auto& Axis : Axises) {
		if (!OverlapOnAxis(*this, Other, Axis)) {
			Collide = false;
			return false;
		}
	}

	Collide = true;
	return true;
}

bool OOBB::CheckCollisionPoint(GLfloat X, GLfloat Y) {
	glm::vec2 D = glm::vec2(X, Y) - Center;

	for (int i = 0; i < 2; ++i) {
		GLfloat Dist = glm::dot(D, Axis[i]);
		if (std::abs(Dist) > Offset[i]) {
			Collide = false;
			return false;
		}
	}

	Collide = true;
	return true;
}



void BoundingSphere::Update(GLfloat X, GLfloat Y, GLfloat SizeValue) {
	Center.x = X;
	Center.y = Y;
	Radius = SizeValue / 2.0;
	Size = SizeValue;
}

void BoundingSphere::Render() {
#ifdef SHOW_BOUND_BOX
	TranslateMatrix = glm::mat4(1.0f);
	ScaleMatrix = glm::mat4(1.0f);

	Transform::Move(TranslateMatrix, Center.x, Center.y);
	Transform::Scale(ScaleMatrix, Size, Size);

	ProcessTransform();

	imageUtil.Render(ImageCollisionSphere);

	if (Collide)
		imageUtil.Render(ImageCollidedSphere);
#endif
}

GLfloat BoundingSphere::CalculateDistance( GLfloat X, GLfloat Y) {
	GLfloat DX = X - Center.x;
	GLfloat DY = Y - Center.y;
	return std::sqrt(DX * DX + DY * DY);
}

bool BoundingSphere::CheckCollision(const BoundingSphere& Other) {
	if (CalculateDistance(Other.Center.x, Other.Center.y) < Radius + Other.Radius) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool BoundingSphere::CheckCollisionPoint(GLfloat X, GLfloat Y) {
	if (CalculateDistance(X, Y) < Radius) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

void BoundingSphere::ProcessTransform() {
#ifdef SHOW_BOUND_BOX
	glUseProgram(ImageShader);
	camera.PrepareRender(SHADER_TYPE_IMAGE);

	TransparencyLocation = glGetUniformLocation(ImageShader, "transparency");
	glUniform1f(TransparencyLocation, 1.0);

	ObjectColorLocation = glGetUniformLocation(ImageShader, "objectColor");
	glUniform3f(ObjectColorLocation, 1.0, 0.0, 0.0);

	BoolBlurLocation = glGetUniformLocation(ImageShader, "UseBlur");
	glUniform1i(BoolBlurLocation, 0);

	ModelLocation = glGetUniformLocation(ImageShader, "model");
	glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, value_ptr(TranslateMatrix * ScaleMatrix));
#endif
}