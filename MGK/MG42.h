#pragma once
#pragma once
#include "GameObject.h"
#include "Flame.h"

class MG42 : public GameObject {
private:
	// �ݹ� Ÿ�̹�
	GLfloat ShootTime{ 0.06 };

	// ���� ��ź��
	int CurrentAmmo{ 150 };

	// �ִ� ��ź ��
	int MaxAmmo{ 150 };

	// ������ �ð�
	GLfloat MaxReloadTime{ 2.5 };

	// ��Ƽ� ��� ����
	bool TriggerState{};

	// ũ�ν���� ������Ʈ ������
	GameObject* CrosshairPtr{};

	// �ε������� ������Ʈ ������
	GameObject* IndPtr{};


	// ��ġ
	glm::vec2 Position{};

	// ȸ�� ��
	GLfloat Rotation{};

	// �ٶ󺸴� ����
	int LookDir{};

	// �ݵ�
	GLfloat RecoilPosition{};

	// �ݹ� Ÿ�̸�
	TimerUtil ShootingTimer;

	// ������ Ÿ�̸�
	TimerUtil reloadTimer;

	// ���� ����� ���� ä��
	SoundChannel ch{};

	// ������ ����
	bool ReloadState{};

public:
	// �� �̸� �Է�, �̸��� ���� �߻� �ӵ��� ��������
	MG42() {
		// ũ�ν� ��� ������ ���
		CrosshairPtr = scene.Find("crosshair");

		// �ε������� �����͸� ��� ���� ��ź���� �����ϱ�
		IndPtr = scene.Find("indammo");
		IndPtr->InputCurrentAmmo(CurrentAmmo);
	}

	GLfloat GetRecoilPosition() {
		return RecoilPosition;
	}

	// ��ġ �Է�, �÷��̾��� ��ġ�� �Է¹޴´�
	void InputPosition(glm::vec2 PositionValue) {
		Position = PositionValue;
	}

	// ���� ������ �Է¹޴´�
	void InputLookDir(int Dir) {
		LookDir = Dir;
	}

	// ȸ�� ������ �Է¹޴´�
	void InputRotation(GLfloat Value) {
		Rotation = Value;
	}

	// ��Ƽ� ��� ���¸� �ο��Ѵ�.
	void InputTriggerState(bool State) {
		TriggerState = State;
	}

	// ���� ������ �Ѵ�.
	void Reload() {
		if (CurrentAmmo < MaxAmmo && !ReloadState) {
			PlaySound(LMG_Reload, ch, 0);

			// ������ ���� Ȱ��ȭ�� �ε������ͷ� ����
			IndPtr->InputReloadState(true);
			ReloadState = true;
		}
	}

	// �������� ������Ʈ �Ѵ�.
	void UpdateReload(float FT) {
		reloadTimer.Update(FT);

		// ������ ��ô���� �ε������ͷ� �����Ѵ�.
		IndPtr->InputReloadProgress(reloadTimer.MiliSec(2), MaxReloadTime);

		// �������� �Ϸ�Ǿ��� ��
		if (reloadTimer.MiliSec(1) >= MaxReloadTime) {
			CurrentAmmo = MaxAmmo;
			IndPtr->InputCurrentAmmo(CurrentAmmo);
			reloadTimer.Reset();

			// ������ ���� ��Ȱ��ȭ�� �ε������ͷ� ����
			IndPtr->InputReloadState(false);
			ReloadState = false;
		}
	}

	void UpdateFunc(float FT) {
		if (ShootingTimer.MiliSec(2) < ShootTime)
			ShootingTimer.Update(FT);

		// ��Ƽ踦 ��� ���¿����� ���� �������� �ݹߵȴ�.
		if (CurrentAmmo != 0 && !ReloadState && TriggerState) {
			if (ShootingTimer.MiliSec(2) >= ShootTime) {

				// �Ҳ� ������Ʈ�� ��ġ�� �ѱ� �տ� ��ġ�ϵ��� ���
				glm::vec2 FlamePosition{};
				FlamePosition.x = Position.x + cos(glm::radians(Rotation)) * 0.5;
				FlamePosition.y = Position.y + sin(glm::radians(Rotation)) * 0.5;

				// �ݹ� �� ���� ��°� �Բ� �Ҳ� ������Ʈ�� �߰��Ѵ�
				PlaySound(MG42_Shoot, ch, 0);
				scene.AddObject(new Flame(FlamePosition.x, FlamePosition.y, Rotation), "flame", LAYER_3);

				// �� ������Ʈ�� �ݵ��� �ش�.
				RecoilPosition = 0.1;

				// ũ�ν����� �ݵ��� �ְ�, �߻� ���¸� �ο��Ѵ�.
				CrosshairPtr->GiveRecoil(0.04);
				CrosshairPtr->ShootGun();

				// ī�޶󿡴� ��鸲 ��ġ�� �߰��Ѵ�.
				cameraCon.AddShakeValue(0.5);

				// ź���� �Һ��ϰ�, ���� ��ź���� �ε������ͷ� �����Ѵ�.
				--CurrentAmmo;
				IndPtr->InputCurrentAmmo(CurrentAmmo);

				// Ÿ�̸� ����
				ShootingTimer.Reset();
			}
		}

		RecoilPosition = Math::Lerp(RecoilPosition, 0.0, 50, FT);

		// ������ ������ ��� ������ ������Ʈ
		if (ReloadState)
			UpdateReload(FT);
	}

	void RenderFunc() {
		InitMatrix();
		Transform::Move(TranslateMatrix, Position.x, Position.y);

		// ���� ����
		if (LookDir == 0) {
			Transform::Rotate(TranslateMatrix, Rotation + 180.0);
			Transform::Move(TranslateMatrix, -0.22 + RecoilPosition, 0.0);
			Flip(FLIP_H);
		}
		// ������ ����
		else if (LookDir == 1) {
			Transform::Rotate(TranslateMatrix, Rotation);
			Transform::Move(TranslateMatrix, 0.22 - RecoilPosition, 0.0);
		}

		Transform::Scale(ScaleMatrix, 0.5, 0.45);
		Render(MG42_Image);
	}
};
