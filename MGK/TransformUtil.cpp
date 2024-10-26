#include "TransformUtil.h"

void Transform::Move(glm::mat4& Matrix, GLfloat X, GLfloat Y) {
	Matrix = translate(Matrix, glm::vec3(X, Y, 0.0));
}

void Transform::Move(glm::mat4& Matrix, glm::vec2 Position) {
	Matrix = translate(Matrix, glm::vec3(Position.x, Position.y, 0.0));
}

void Transform::Rotate(glm::mat4& Matrix, GLfloat Degree) {
	Matrix = rotate(Matrix, glm::radians(Degree), glm::vec3(0.0, 0.0, 1.0));
}

void Transform::RotateRad(glm::mat4& Matrix, GLfloat Radians) {
	Matrix = rotate(Matrix, Radians, glm::vec3(0.0, 0.0, 1.0));
}

void Transform::RotateV(glm::mat4& Matrix, GLfloat Degree) {
	Matrix = rotate(Matrix, glm::radians(Degree), glm::vec3(1.0, 0.0, 0.0));
}

void Transform::RotateH(glm::mat4& Matrix, GLfloat Degree) {
	Matrix = rotate(Matrix, glm::radians(Degree), glm::vec3(0.0, 1.0, 0.0));
}

void Transform::Scale(glm::mat4& Matrix, GLfloat X, GLfloat Y) {
	Matrix = scale(Matrix, glm::vec3(X, Y, 1.0));
}

void Transform::Scale(glm::mat4& Matrix, glm::vec2 Size) {
	Matrix = scale(Matrix, glm::vec3(Size.x, Size.y, 1.0));
}