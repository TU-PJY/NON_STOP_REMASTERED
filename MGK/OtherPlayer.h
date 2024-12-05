#pragma once
#include "GameObject.h"

enum EnumLookDir {
	LOOK_LEFT,
	LOOK_RIGHT
};

class OtherPlayer : public GameObject {
public:
	glm::vec2 Position{};
	std::string GunType{};
	std::string Tag{};
	GLfloat GunRotation{};
	bool LookDir{};

	OtherPlayer() {

	}

	void SetPosition(glm::vec2 Value) {
		Position = Value;
	}

	void SetGunType(std::string Type) {
		GunType = Type;
	}

	void SetPlayerTag(std::string Str) {
		Tag = Str;
	}

	void SetLookDir(int Dir) {
		LookDir = Dir;
	}

	void SetGunRotation(GLfloat Rotation) {
		GunRotation = Rotation;
	}

	int GetLookDir() { 
		return LookDir;
	}

	GLfloat GetGunRotation() { 
		return GunRotation;
	}

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

		// ������ �� ��� 
		if (LookDir == LOOK_LEFT)
			Flip(FLIP_H);

		// �������� �ʿ��� �����͵��� ���̴��� ���� �� ���� ������
		Render(PlayerImage);
	}
};