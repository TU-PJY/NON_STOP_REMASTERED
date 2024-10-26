#include "UiUtil.h"

void UI::ClampMatrixToCorner(int EdgePosition, glm::mat4& Matrix, GLfloat Width, GLfloat Height, GLfloat GapX, GLfloat GapY) {
	switch (EdgePosition) {
	case EDGE_LEFT_DOWN:
		Matrix = translate(Matrix, glm::vec3(WindowRect.lx + Width / 2.0 + GapX, WindowRect.ly + Height / 2.0 + GapY, 0.0));
		break;

	case EDGE_LEFT_UP:
		Matrix = translate(Matrix, glm::vec3(WindowRect.lx + Width / 2.0 + GapX, WindowRect.ry - Height / 2.0 - GapY, 0.0));
		break;

	case EDGE_RIGHT_DOWN:
		Matrix = translate(Matrix, glm::vec3(WindowRect.rx - Width / 2.0 - GapX, WindowRect.ly + Height / 2.0 + GapY, 0.0));
		break;

	case EDGE_RIGHT_UP:
		Matrix = translate(Matrix, glm::vec3(WindowRect.rx - Width / 2.0 - GapX, WindowRect.ry - Height / 2.0 - GapY, 0.0));
		break;
	}
}

void UI::ClampMatrixToPosition(glm::mat4& Matrix, GLfloat PositionX, GLfloat PositionY, GLfloat Width, GLfloat Height, GLfloat GapX, GLfloat GapY) {
	Matrix = translate(Matrix, glm::vec3(PositionX + Width / 2.0 + GapX, PositionY + Height / 2.0 + GapY, 0.0));
}

void UI::ClampPositionToCorner(int EdgePosition, GLfloat& PositionX, GLfloat& PositionY, GLfloat Width, GLfloat Height, GLfloat GapX, GLfloat GapY) {
	switch (EdgePosition) {
	case EDGE_LEFT_DOWN:
		PositionX = WindowRect.lx + Width / 2.0 + GapX;
		PositionY = WindowRect.ly + Height / 2.0 + GapY;
		break;

	case EDGE_LEFT_UP:
		PositionX = WindowRect.lx + Width / 2.0 + GapX;
		PositionY = WindowRect.ry - Height / 2.0 - GapY;
		break;

	case EDGE_RIGHT_DOWN:
		PositionX = WindowRect.rx - Width / 2.0 - GapX;
		PositionY = WindowRect.ly + Height / 2.0 + GapY;
		break;

	case EDGE_RIGHT_UP:
		PositionX = WindowRect.rx - Width / 2.0 - GapX;
		PositionY = WindowRect.ry - Height / 2.0 - GapY;
		break;
	}
}

void UI::ClampPositionToCoord(GLfloat& ThisPositionX, GLfloat& ThisPositionY, GLfloat OtherPositionX, GLfloat OtherPositionY, GLfloat Width, GLfloat Height, GLfloat GapX, GLfloat GapY) {
	ThisPositionX = OtherPositionX + Width / 2.0 + GapX;
	ThisPositionY = OtherPositionY + Height / 2.0 + GapY;
}