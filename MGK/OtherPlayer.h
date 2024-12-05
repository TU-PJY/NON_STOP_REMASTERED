#pragma once
#include "GameObject.h"

class OtherPlayer : public GameObject {
public:
	void SetPosition(glm::vec2 Value) {
		Position = Value;
	}

	void SetGunType(std::string Type) {
		GunType = Type;
	}

	void SetPlayerTag(std::string Str) {
		Tag = Str;
	}

	glm::vec2 Position{};
	std::string GunType{};
	std::string Tag{};

	void UpdateFunc(float FT) {
		//std::cout << ObjectTag << std::endl;
	}

	void RenderFunc() {
		// 행렬 초기화, 렌더링 전 반드시 실행해야한다.
		InitMatrix();

		// 플레이어 위치 값만큼 이동
		Transform::Move(TranslateMatrix, Position);

		// 플레이어 출력 크기 조정
		Transform::Scale(ScaleMatrix, 0.3, 0.3);

		// 렌더링에 필요한 데이터들을 쉐이더로 전달 후 최종 렌더링
		Render(PlayerImage);
	}
};