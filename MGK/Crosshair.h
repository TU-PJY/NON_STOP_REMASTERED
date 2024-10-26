#pragma once
#include "GameObject.h"
#include "MouseUtil.h"
#include "CameraController.h"

class Crosshair : public GameObject {
private:
	// 크로스 헤어가 표시되는 위치
	glm::vec2 Position{};

	// 월드 상의 크로스헤어 위치
	glm::vec2 RealPosition{};

	// 랜덤 생성 타격 지점
	glm::vec2 RandomTarget{};

	// 반동 수치
	GLfloat Recoil{};

	// 선 그리기 클래스
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
		// 크로스 헤어가 표시되는 위치 지정
		Position.x = mouse.x;
		Position.y = mouse.y;

		// 월드 상의 크로스헤어 위치 지정
		RealPosition.x = mouse.x + cameraCon.Position.x;
		RealPosition.y = mouse.y;

		// 반동 수치를 점차 줄인다
		Recoil -= FT * 0.5;

		// Recoil이 0.0 미만으로 감소하지 않도록 한다.
		EX::ClampValue(Recoil, 0.0, CLAMP_LESS);
	}

	void RenderFunc() {
		// left
		line.Draw(Position.x - 0.15 - Recoil, Position.y, Position.x - 0.03 - Recoil, Position.y, 0.015);

		// right
		line.Draw(Position.x + 0.03 + Recoil, Position.y, Position.x + 0.15 + Recoil, Position.y, 0.015);

		// upper
		line.Draw(Position.x, Position.y + 0.03 + Recoil, Position.x, Position.y + 0.15 + Recoil, 0.015);

		// under
		line.Draw(Position.x, Position.y - 0.15 - Recoil, Position.x, Position.y - 0.03 - Recoil, 0.015);
	}
};