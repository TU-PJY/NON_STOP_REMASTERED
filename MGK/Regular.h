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

	// 대미지 쿨타임 타이머
	TimerUtil DamageTimer;
	GLfloat CoolTime{ 0.7 };

	// 대미지 가능 여부
	bool DamageEnableState{ true };

	std::string Type{ "regular" };

	// 대미지 받았음을 표시하는 빨간 색
	GLfloat RedColor{};

	// 대미지 가하기 피드백
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
		// 체력이 0이 되면 스스로 삭제
		if (HP <= 0) {
			// 삭제할 몬스터 목록에 자신의 ID 추가
			DeleteMonsterList.emplace_back(ID);
			scene.DeleteObject(this);
		}

		if (auto player = scene.Find("player"); player) {
			glm::vec2 PlayerPosition = player->GetPosition();

			if (Position.x < PlayerPosition.x)
				LookDir = 1;

			if (Position.x > PlayerPosition.x)
				LookDir = -1;

			// 플레이어 - 몬스터 충돌 처리
			if (player->GetAABB().CheckCollision(aabb)) {
				if (DamageEnableState) {
					player->GiveDamage(8);
					FeedbackHeight = 0.2;
					DamageEnableState = false;
					DamageTimer.Reset();
				}
			}

			// 충돌하지 않을 때만 이동한다.
			else
				Position.x += Speed * LookDir * FT;
		}

		// aabb 업데이트
		aabb.Update(Position.x, Position.y, 0.25, 0.3);

		// 대미지 쿨타임을 업데이트 한다. 
		if(!DamageEnableState)
			DamageTimer.Update(FT);
		if (DamageTimer.MiliSec(2) >= CoolTime)
			DamageEnableState = true;

		// 대미지 피드백 업데이트
		RedColor = Math::Lerp(RedColor, 0.0, 2.5, FT);

		// 대미지 가하기 피드백 업데이트
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