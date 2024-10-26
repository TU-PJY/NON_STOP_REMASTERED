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

public:
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
	}

	void RenderFunc() {
		InitMatrix();
		Transform::Move(TranslateMatrix, Position);
		Transform::Scale(ScaleMatrix, 0.3, 0.3);

		if (LookDir == -1)
			Flip(FLIP_H);

		Render(RegularMonsterImage);
		aabb.Render();
	}
};