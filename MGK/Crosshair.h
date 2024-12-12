#pragma once
#include "GameObject.h"
#include "MouseUtil.h"
#include "CameraController.h"
#include "Scene.h"

class Crosshair : public GameObject {
private:
	// ũ�ν� �� ǥ�õǴ� ��ġ
	glm::vec2 Position{};

	// ���� ���� ũ�ν���� ��ġ
	glm::vec2 RealPosition{};

	// ���� ���� Ÿ�� ����
	glm::vec2 TargetPoint{};

	// �ݵ� ��ġ
	GLfloat Recoil{};

	// �� �׸��� Ŭ����
	LineBrush line;

	// �ݹ� ����
	bool ShootState{};

public:
	Crosshair() {
		line.SetColor(1.0, 0.0, 0.0);

		// ī�޶��� ���¿� ������ ���� �ʴ´�.
		line.SetRenderType(RENDER_TYPE_STATIC);
	}

	// ũ�ν��� �ݵ��� �ش�
	void GiveRecoil(GLfloat Value) {
		Recoil += Value;
	}

	// �Ѿ��� �߻��Ѵ�.
	void ShootGun() {
		// ������ ������ �����Ѵ�.
		TargetPoint.x = Random::Gen(DIST_REAL, RealPosition.x - 0.03 - Recoil, RealPosition.x + 0.03 + Recoil);
		TargetPoint.y = Random::Gen(DIST_REAL, RealPosition.y - 0.03 - Recoil, RealPosition.y + 0.03 + Recoil);

		// monster �±׸� ���� ������ ��´�.
		ObjectRange MonsterRange = scene.EqualRange("monster");

		// ���� ���� �ִ� ��� ������Ʈ�� AABB �浹 �˻縦 ���� ����� �ο� ���θ� �����Ѵ�.
		for (auto It = MonsterRange.First; It != MonsterRange.End; ++It) {
			if (It->second->GetAABB().CheckCollisionPoint(TargetPoint.x, TargetPoint.y)) {
				int Damage{};

				// �÷��̾��� �ѿ� ���� ������� �ٸ��� �ο��ȴ�.
				if (PlayerGunType == "SCAR_H")
					Damage = 25;

				else if(PlayerGunType == "M16")
					Damage = 18;

				else if(PlayerGunType == "MP44")
					Damage = 35;

				else if (PlayerGunType == "MG42")
					Damage = 10;

				It->second->GiveDamage(Damage);

				// ������� ���� ���� ����Ʈ�� ���̵� �� ����� �߰� 
				DamageMonsterList.push_back({ It->second->GetID(), Damage });
			}
		}
	}

	void UpdateFunc(float FT) {
		// ũ�ν� �� ǥ�õǴ� ��ġ ����
		Position.x = mouse.x + cameraCon.Shake.x;
		Position.y = mouse.y + cameraCon.Shake.y - cameraCon.PushValue;

		// ���� ���� ũ�ν���� ��ġ ����
		RealPosition.x = mouse.x - cameraCon.Position.x;
		RealPosition.y = mouse.y;

		// �ݵ� ��ġ�� ���� ���δ�
		Recoil -= FT * 0.5;

		// Recoil�� 0.0 �̸����� �������� �ʵ��� �Ѵ�.
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