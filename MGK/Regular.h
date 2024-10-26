#pragma once
#include "GameObject.h"
#include "Scene.h"

class RegularMonster : public GameObject {
private:
	glm::vec2 Position{};
	int HP{ 100 };
	int LookDir{};
	GLfloat Speed{ 1.0 };
	AABB aabb;

	// 대미지 받았음을 표시하는 빨간 색
	GLfloat RedColor{};

public:
	AABB GetAABB() {
		return aabb;
	}

	void GiveDamage(int Damage) {
		HP -= Damage;
		RedColor = 1.0;
	}

	RegularMonster(int GenDir) {
		if (GenDir == 0)
			Position.x = -8.0;

		else if (GenDir == 1)
			Position.x = 8.0;

		Position.y = -0.5;
	}

	void UpdateFunc(float FT) {
		if (auto player = scene.Find("player"); player) {
			glm::vec2 PlayerPosition = player->GetPosition();

			if (Position.x < PlayerPosition.x)
				LookDir = 1;

			if (Position.x > PlayerPosition.x)
				LookDir = -1;

			Position.x += Speed * LookDir * FT;
		}

		// 보는 방향으로 이동
		aabb.Update(Position.x, Position.y, 0.25, 0.3);

		RedColor = Math::Lerp(RedColor, 0.0, 2.5, FT);

		if (HP <= 0)
			scene.DeleteObject(this);
	}

	void RenderFunc() {
		SetColor(RedColor, 0.0, 0.0);

		InitMatrix();
		Transform::Move(TranslateMatrix, Position);
		Transform::Scale(ScaleMatrix, 0.3, 0.3);

		if (LookDir == -1)
			Flip(FLIP_H);

		Render(RegularMonsterImage);
		aabb.Render();
	}
};