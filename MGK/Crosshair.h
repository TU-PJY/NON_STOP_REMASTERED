#pragma once
#include "GameObject.h"
#include "MouseUtil.h"
#include "CameraController.h"

class Crosshair : public GameObject {
private:
	glm::vec2 Position{};
	glm::vec2 RandomTarget{};
	GLfloat Recoil{};
	LineBrush line;

public:
	Crosshair() {
		line.SetColor(1.0, 0.0, 0.0);
		// 카메라의 상태에 영향을 받지 않는다.
		line.SetRenderType(RENDER_TYPE_STATIC);
	}

	// 크로스헤어에 반동을 준다
	void GiveRecoil(GLfloat Value) {
		Recoil += Value;
	}

	void UpdateFunc(float FT) {
		Position.x = mouse.x;
		Position.y = mouse.y;

		Recoil -= FT * 0.5;
		EX::ClampValue(Recoil, 0.0, CLAMP_LESS);
	}

	void RenderFunc() {
		// left
		line.Draw(Position.x - 0.1 - Recoil, Position.y, Position.x - 0.05 - Recoil, Position.y, 0.007);

		// right
		line.Draw(Position.x + 0.05 + Recoil, Position.y, Position.x + 0.1 + Recoil, Position.y, 0.007);

		// upper
		line.Draw(Position.x, Position.y + 0.05 + Recoil, Position.x, Position.y + 0.1 + Recoil, 0.007);

		// under
		line.Draw(Position.x, Position.y - 0.05 - Recoil, Position.x, Position.y - 0.1 - Recoil, 0.007);
	}
};