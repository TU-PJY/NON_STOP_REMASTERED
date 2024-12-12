#pragma once
#include "GameObject.h"
#include "Scene.h"
#include "TCP_Header.h"

class RegularMonster : public GameObject {
private:
	glm::vec2 Position{};
	int HP{ 100 };
	int LookDir{};
	GLfloat Speed{ 1.0 };
	AABB aabb;

	int ID{};

	// ����� ��Ÿ�� Ÿ�̸�
	TimerUtil DamageTimer;
	GLfloat CoolTime{ 0.7 };

	// ����� ���� ����
	bool DamageEnableState{ true };

	std::string Type{ "regular" };

	// ����� �޾����� ǥ���ϴ� ���� ��
	GLfloat RedColor{};

	// ����� ���ϱ� �ǵ��
	GLfloat FeedbackHeight{};

public:
	AABB GetAABB() {
		return aabb;
	}

	void GiveDamage(int Damage) {
		HP -= Damage;
		RedColor = 1.0;
	}

	int GetID() {
		return ID;
	}

	RegularMonster(int GenDir, int IDValue) {
		if (GenDir == 0)
			Position.x = -8.0;

		else if (GenDir == 1)
			Position.x = 8.0;

		Position.y = -0.5;

		ID = IDValue;
	}

	void UpdateFunc(float FT) {
		// ü���� 0�� �Ǹ� ������ ����
		if (HP <= 0) {
			// ������ ���� ��Ͽ� �ڽ��� ID �߰�
			DeleteMonsterList.emplace_back(ID);
			scene.DeleteObject(this);
		}

		if (auto player = scene.Find("player"); player) {
			glm::vec2 PlayerPosition = player->GetPosition();

			if (Position.x < PlayerPosition.x)
				LookDir = 1;

			if (Position.x > PlayerPosition.x)
				LookDir = -1;

			// �÷��̾� - ���� �浹 ó��
			if (player->GetAABB().CheckCollision(aabb)) {
				if (DamageEnableState) {
					player->GiveDamage(8);
					FeedbackHeight = 0.2;
					DamageEnableState = false;
					DamageTimer.Reset();
				}
			}

			// �浹���� ���� ���� �̵��Ѵ�.
			else
				Position.x += Speed * LookDir * FT;
		}

		// aabb ������Ʈ
		aabb.Update(Position.x, Position.y, 0.25, 0.3);

		// ����� ��Ÿ���� ������Ʈ �Ѵ�. 
		if(!DamageEnableState)
			DamageTimer.Update(FT);
		if (DamageTimer.MiliSec(2) >= CoolTime)
			DamageEnableState = true;

		// ����� �ǵ�� ������Ʈ
		RedColor = Math::Lerp(RedColor, 0.0, 2.5, FT);

		// ����� ���ϱ� �ǵ�� ������Ʈ
		FeedbackHeight = Math::Lerp(FeedbackHeight, 0.0, 10, FT);
	}

	void RenderFunc() {
		SetColor(RedColor, 0.0, 0.0);

		InitMatrix();
		Transform::Move(TranslateMatrix, Position.x, Position.y + FeedbackHeight * 0.5);
		Transform::Scale(ScaleMatrix, 0.3, 0.3 + FeedbackHeight);

		if (LookDir == -1)
			Flip(FLIP_H);

		Render(RegularMonsterImage);
		aabb.Render();
	}
};