#pragma once
#pragma once
#include "GameObject.h"
#include "Flame.h"

class MG42 : public GameObject {
private:
	// 격발 타이밍
	GLfloat ShootTime{ 0.06 };

	// 현재 장탄수
	int CurrentAmmo{ 150 };

	// 최대 장탄 수
	int MaxAmmo{ 150 };

	// 재장전 시간
	GLfloat MaxReloadTime{ 2.5 };

	// 방아쇠 당김 여부
	bool TriggerState{};

	// 크로스헤어 오브젝트 포인터
	GameObject* CrosshairPtr{};

	// 인디케이터 오브젝트 포인터
	GameObject* IndPtr{};


	// 위치
	glm::vec2 Position{};

	// 회전 값
	GLfloat Rotation{};

	// 바라보는 방향
	int LookDir{};

	// 반동
	GLfloat RecoilPosition{};

	// 격발 타이머
	TimerUtil ShootingTimer;

	// 재장전 타이머
	TimerUtil reloadTimer;

	// 사운드 출력을 위한 채널
	SoundChannel ch{};

	// 재장전 여부
	bool ReloadState{};

public:
	// 총 이름 입력, 이름에 따라 발사 속도가 정해진다
	MG42() {
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
		if (CurrentAmmo < MaxAmmo && !ReloadState) {
			PlaySound(LMG_Reload, ch, 0);

			// 재장전 상태 활성화를 인디케이터로 전송
			IndPtr->InputReloadState(true);
			ReloadState = true;
		}
	}

	// 재장전을 업데이트 한다.
	void UpdateReload(float FT) {
		reloadTimer.Update(FT);

		// 재장전 진척도를 인디케이터로 전송한다.
		IndPtr->InputReloadProgress(reloadTimer.MiliSec(2), MaxReloadTime);

		// 재장전이 완료되었을 때
		if (reloadTimer.MiliSec(1) >= MaxReloadTime) {
			CurrentAmmo = MaxAmmo;
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
		if (CurrentAmmo != 0 && !ReloadState && TriggerState) {
			if (ShootingTimer.MiliSec(2) >= ShootTime) {

				// 불꽃 오브젝트의 위치가 총구 앞에 위치하도록 계산
				glm::vec2 FlamePosition{};
				FlamePosition.x = Position.x + cos(glm::radians(Rotation)) * 0.5;
				FlamePosition.y = Position.y + sin(glm::radians(Rotation)) * 0.5;

				// 격발 시 사운드 출력과 함께 불꽃 오브젝트를 추가한다
				PlaySound(MG42_Shoot, ch, 0);
				scene.AddObject(new Flame(FlamePosition.x, FlamePosition.y, Rotation), "flame", LAYER_3);

				// 총 오브젝트에 반동을 준다.
				RecoilPosition = 0.1;

				// 크로스헤어에는 반동을 주고, 발사 상태를 부여한다.
				CrosshairPtr->GiveRecoil(0.04);
				CrosshairPtr->ShootGun();

				// 카메라에는 흔들림 수치를 추가한다.
				cameraCon.AddShakeValue(0.5);

				// 탄약을 소비하고, 현재 장탄수를 인디케이터로 전송한다.
				--CurrentAmmo;
				IndPtr->InputCurrentAmmo(CurrentAmmo);

				// 타이머 리셋
				ShootingTimer.Reset();
			}
		}

		RecoilPosition = Math::Lerp(RecoilPosition, 0.0, 50, FT);

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
			Transform::Move(TranslateMatrix, -0.22 + RecoilPosition, 0.0);
			Flip(FLIP_H);
		}
		// 오른쪽 방향
		else if (LookDir == 1) {
			Transform::Rotate(TranslateMatrix, Rotation);
			Transform::Move(TranslateMatrix, 0.22 - RecoilPosition, 0.0);
		}

		Transform::Scale(ScaleMatrix, 0.5, 0.45);
		Render(MG42_Image);
	}
};
