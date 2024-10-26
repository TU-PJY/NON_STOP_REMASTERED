#pragma once
#include "GameObject.h"
#include "MouseUtil.h"
#include "CameraController.h"

class Crosshair : public GameObject {
private:
	glm::vec2 Position{};
	bool ActivateState{};

	LineBrush line;

public:
	Crosshair() {
		line.SetColor(0.0, 1.0, 0.0);

		// 카메라의 상태에 영향을 받지 않는다.
		line.SetRenderType(RENDER_TYPE_STATIC);
	}

	// 크로스헤어에 반동을 준다
	void GiveRecoil(GLfloat Value) {
		mouse.CrossY += Value;
		GLfloat RecoilH = Random::Gen(DIST_REAL, -Value, Value);
		mouse.CrossX += RecoilH;
	}

	void UpdateFunc(float FT) {
		if (ActivateState) {
			Position.x = mouse.CrossX;
			Position.y = mouse.CrossY;

			if (Position.x > ASP(1.0)) {
				Position.x = ASP(1.0);
				mouse.CrossX = ASP(1.0);
			}
			else if (Position.x < ASP(-1.0)) {
				Position.x = ASP(-1.0);
				mouse.CrossX = ASP(-1.0);
			}
			if (Position.y > 1.0) {
				Position.y = 1.0;
				mouse.CrossY = 1.0;
			}
			else if (Position.y < -1.0) {
				Position.y = -1.0;
				mouse.CrossY = -1.0;
			}
		}

		else {
			Position.x = 0.0;
			Position.y = 0.0;
			ActivateState = true;
		}
	}

	void RenderFunc() {
		// left
		line.Draw(Position.x - 0.1, Position.y, Position.x - 0.05, Position.y, 0.007);

		// right
		line.Draw(Position.x + 0.05, Position.y, Position.x + 0.1, Position.y, 0.007);

		// upper
		line.Draw(Position.x, Position.y + 0.05, Position.x, Position.y + 0.1, 0.007);

		// under
		line.Draw(Position.x, Position.y - 0.05, Position.x, Position.y - 0.1, 0.007);
	}
};