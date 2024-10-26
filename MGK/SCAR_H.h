#pragma once
#include "GameObject.h"
#include "Flame.h"

class SCAR_H : public GameObject {
private:
	// 위치
	glm::vec2 Position{};

	// 바라보는 방향
	int LookDir{};

	// 회전 값
	GLfloat Rotation{};

	// 반동
	GLfloat Recoil{};

	// 격발 타이밍 타이머
	TimerUtil ShootingTimer;

	// 격발 타이밍
	GLfloat ShootTime{};

	// 방아쇠 당김 여부
	bool TriggerState{};

	// 사운드 출력을 위한 채널
	SoundChannel ch{};

	GameObject* CrosshairPtr{};

public:
	// 총 이름 입력, 이름에 따라 발사 속도가 정해진다
	SCAR_H(GLfloat Time) {
		ShootTime = Time;
		if (auto crosshair = scene.Find("crosshair"); crosshair)
			CrosshairPtr = crosshair;
	}

	// 위치 입력, 플레이어의 위치를 입력받는다
	void InputPosition(glm::vec2 PositionValue) {
		Position = PositionValue;
	}

	// 보는 방향을 입력받는다
	void InputLookDir(int Dir) {
		LookDir = Dir;
	}

	// 회전 각도를 입력받는다
	void InputRotation(GLfloat Value) {
		Rotation = Value;
	}

	void InputTriggerState(bool State) {
		TriggerState = State;
	}

	void UpdateFunc(float FT) {
		if (ShootingTimer.MiliSec(2) < ShootTime)
			ShootingTimer.Update(FT);

		// 방아쇠를 당긴 상태에서는 일정 간격으로 격발된다.
		// 격발 시 사운드 출력과 함께 불꽃 오브젝트를 추가한다
		// 크로스헤어에는 반동을 준다.
		if (TriggerState) {
			if (ShootingTimer.MiliSec(2) >= ShootTime) {
				ShootingTimer.Reset();
				PlaySound(SCAR_H_Shoot, ch, 0);
				scene.AddObject(new Flame(
					Position.x + cos(glm::radians(Rotation)) * 0.35, 
					Position.y + sin(glm::radians(Rotation)) * 0.35,
					Rotation
				), "flame", LAYER_3);

				Recoil = 0.1;

				CrosshairPtr->GiveRecoil(0.08);
			}
		}

		Recoil = Math::Lerp(Recoil, 0.0, 15, FT);
	}

	void RenderFunc() {
		InitMatrix();
		Transform::Move(TranslateMatrix, Position.x, Position.y);

		// 왼쪽 방향
		if (LookDir == 0) {
			Transform::Rotate(TranslateMatrix, Rotation + 180.0);
			Transform::Move(TranslateMatrix, -0.15 + Recoil, 0.0);
			Flip(FLIP_H);
		}
		// 오른쪽 방향
		else if (LookDir == 1) {
			Transform::Rotate(TranslateMatrix, Rotation);
			Transform::Move(TranslateMatrix, 0.15 - Recoil, 0.0);
		}

		Transform::Scale(ScaleMatrix, 0.18, 0.18);
		Render(SCAR_H_Image);
	}
};