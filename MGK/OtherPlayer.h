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
		// ��� �ʱ�ȭ, ������ �� �ݵ�� �����ؾ��Ѵ�.
		InitMatrix();

		// �÷��̾� ��ġ ����ŭ �̵�
		Transform::Move(TranslateMatrix, Position);

		// �÷��̾� ��� ũ�� ����
		Transform::Scale(ScaleMatrix, 0.3, 0.3);

		// �������� �ʿ��� �����͵��� ���̴��� ���� �� ���� ������
		Render(PlayerImage);
	}
};