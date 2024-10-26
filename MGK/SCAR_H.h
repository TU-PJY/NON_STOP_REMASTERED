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
	GLfloat RecoilPosition{};

	// 격발 타이밍 타이머
	TimerUtil ShootingTimer;

	// 격발 타이밍
	GLfloat ShootTime{};

	// 방아쇠 당김 여부
	bool TriggerState{};

	// 사운드 출력을 위한 채널
	SoundChannel ch{};

	// 크로스헤어 오브젝트 포인터
	GameObject* CrosshairPtr{};

	// 인디케이터 오브젝트 포인터
	GameObject* IndPtr{};

	// 현재 장탄수
	int CurrentAmmo{25};

	// 재장전 여부
	bool ReloadState{};

	// 재장전 타미어
	TimerUtil reloadTimer;

public:
	// 총 이름 입력, 이름에 따라 발사 속도가 정해진다
	SCAR_H(GLfloat Time) {
		ShootTime = Time;

		// 크로스 헤어 포인터 얻기
		CrosshairPtr = scene.Find("crosshair");

		// 인디케이터 포인터를 얻고 현재 장탄수를 전송하기
		IndPtr = scene.Find("indammo");
		IndPtr->InputCurrentAmmo(CurrentAmmo);
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

	// 방아쇠 당김 상태를 부여한다.
	void InputTriggerState(bool State) {
		TriggerState = State;
	}

	// 총을 재장전 한다.
	void Reload() {
		if (CurrentAmmo < 25 && !ReloadState) {
			PlaySound(AR_Reload, ch, 0);

			// 재장전 상태 활성화를 인디케이터로 전송
			IndPtr->InputReloadState(true);
			ReloadState = true;
		}
	}

	// 재장전을 업데이트 한다.
	void UpdateReload(float FT) {
		reloadTimer.Update(FT);

		// 재장전 진척도를 인디케이터로 전송한다.
		IndPtr->InputReloadProgress(reloadTimer.MiliSec(2), 0.7);

		if (reloadTimer.MiliSec(1) >= 0.7) {
			CurrentAmmo = 25;
			IndPtr->InputCurrentAmmo(CurrentAmmo);
			reloadTimer.Reset();

			// 재장전 상태 비활성화를 인디케이터로 전송
			IndPtr->InputReloadState(false);
			ReloadState = false;
		}
	}

	void UpdateFunc(float FT) {
		if (ShootingTimer.MiliSec(2) < ShootTime)
			ShootingTimer.Update(FT);

		// 방아쇠를 당긴 상태에서는 일정 간격으로 격발된다.
		// 격발 시 사운드 출력과 함께 불꽃 오브젝트를 추가한다
		// 크로스헤어에는 반동을 준다.
		// 카메라에는 흔들림 수치를 추가한다.
		if (CurrentAmmo != 0 && !ReloadState && TriggerState) {
			if (ShootingTimer.MiliSec(2) >= ShootTime) {
				PlaySound(SCAR_H_Shoot, ch, 0);

				scene.AddObject(new Flame(
					Position.x + cos(glm::radians(Rotation)) * 0.35, 
					Position.y + sin(glm::radians(Rotation)) * 0.35,
					Rotation
				), "flame", LAYER_3);

				RecoilPosition = 0.1;
				CrosshairPtr->GiveRecoil(0.07);
				cameraCon.AddShakeValue(1.0);

				// 크로스헤어에 발사 상태를 부여한다.
				CrosshairPtr->ShootGun();

				// 탄약을 소비한다.
				--CurrentAmmo;

				// 인디케이터로 현재 장탄수 전송
				IndPtr->InputCurrentAmmo(CurrentAmmo);

				ShootingTimer.Reset();
			}
		}

		RecoilPosition = Math::Lerp(RecoilPosition, 0.0, 15, FT);

		// 제장전 상태일 경우 재장전 업데이트
		if (ReloadState)
			UpdateReload(FT);
	}

	void RenderFunc() {
		InitMatrix();
		Transform::Move(TranslateMatrix, Position.x, Position.y);

		// 왼쪽 방향
		if (LookDir == 0) {
			Transform::Rotate(TranslateMatrix, Rotation + 180.0);
			Transform::Move(TranslateMatrix, -0.15 + RecoilPosition, 0.0);
			Flip(FLIP_H);
		}
		// 오른쪽 방향
		else if (LookDir == 1) {
			Transform::Rotate(TranslateMatrix, Rotation);
			Transform::Move(TranslateMatrix, 0.15 - RecoilPosition, 0.0);
		}

		Transform::Scale(ScaleMatrix, 0.36, 0.36);
		Render(SCAR_H_Image);
	}
};