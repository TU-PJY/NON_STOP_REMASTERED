#pragma once
#include "EngineHeader.h"

enum class Edge
{Left, Right, Top, Bottom};


class AABB {
private:
	glm::mat4 TranslateMatrix{ 1.0f }, ScaleMatrix{ 1.0f }, RotateMatrix{ 1.0f };
	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{};
	unsigned int Box{};
	unsigned int BoxInside{};

	GLfloat LeftX{}, LeftY{};
	GLfloat RightX{}, RightY{};
	GLfloat CenterX{}, CenterY{};
	GLfloat OffsetX{}, OffsetY{};

	bool Collide{};

public:
	void Init();
	void Update(GLfloat X, GLfloat Y, GLfloat xScale, GLfloat yScale);
	void BeginProcess();
	void SetPosition(GLfloat X, GLfloat Y);
	void Scale(GLfloat BoxWidth, GLfloat BoxHeight);
	void RotateAxis(GLfloat RotationValue, GLfloat AxisX, GLfloat AxisY);
	void Render(bool Lock=false);
	bool CheckCollision(const AABB& Other);
	bool CheckCollisionEdge(GLfloat X, Edge Edge);
	void InterpolateX(GLfloat& X);
	void InterpolateY(GLfloat& Y);
	bool CheckCollisionPoint(GLfloat X, GLfloat Y);

private:
	void ProcessTransform();
};

class OBB {
private:
	glm::mat4 TranslateMatrix{ 1.0f }, RotateMatrix{ 1.0f }, ScaleMatrix{1.0f};
	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{};
	unsigned int Box{};
	unsigned int BoxInside{};

	glm::vec2 Center{};
	glm::vec2 Offset{};
	glm::vec2 Axis[2]{};
	GLfloat Rotation{};

	bool Collide{};

public:
	void Init();
	void Update(GLfloat X, GLfloat Y, GLfloat BoxWidth, GLfloat BoxHeight, GLfloat RotationValue);
	void BeginProcess();
	void SetPosition(GLfloat X, GLfloat Y);
	void Rotate(GLfloat RotationValue);
	void Scale(GLfloat BoxWidth, GLfloat BoxHeight);
	void Render();
	void RotateAxis(GLfloat RotationValue, GLfloat AxisX, GLfloat AxisY);
	bool CheckCollision(const OBB& Other);
	bool CheckCollisionPoint(GLfloat X, GLfloat Y);

private:
	bool OverlapOnAxis(const OBB& OBB1, const OBB& OBB2, const glm::vec2& Axis);
	void ProcessTransform();
};

class Range {
private:
	glm::mat4 TranslateMatrix{ 1.0f }, ScaleMatrix{ 1.0f }, RotateMatrix{ 1.0f };
	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{};
	unsigned int Circle{};
	unsigned int CircleInside{};

	GLfloat CenterX{}, CenterY{};
	GLfloat Radius{};

	bool Collide{};

public:
	void Init();
	void Update(GLfloat X, GLfloat Y, GLfloat Size);
	void BeginProcess();
	void SetPosition(GLfloat X, GLfloat Y);
	void Scale(GLfloat Size);
	void RotateAxis(GLfloat RotationValue, GLfloat AxisX, GLfloat AxisY);
	void Render();
	bool CheckCollision(const Range& Other);
	bool CheckCollisionPoint(GLfloat X, GLfloat Y);

private:
	GLfloat CalculateDistance(GLfloat x2, GLfloat y2);
	void ProcessTransform();
};