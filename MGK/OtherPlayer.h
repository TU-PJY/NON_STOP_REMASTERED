#pragma once
#include "GameObject.h"
#include "TextUtil.h"

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
	GLfloat RecoilPosition{};
	bool LookDir{};

	TextUtil Text{};

	bool TextInit{};

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

	void SetRecoilPosition(GLfloat Value) {
		RecoilPosition = Value;
	}

	std::wstring ToWstr(const std::string& str) {
		int SizeNeed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
		std::wstring Wstr(SizeNeed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &Wstr[0], SizeNeed);

		return Wstr;
	}

	void UpdateFunc(float FT) {
		if (!TextInit) {
			Text.Init(L"���� ���", FW_BOLD);
			Text.SetAlign(ALIGN_MIDDLE);
			Text.SetColor(0.0, 0.0, 0.0);
			Text.SetRenderType(RENDER_TYPE_DEFAULT);
			TextInit = true;
		}
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
		


		// Ÿ �÷��̾� �� ���
		InitMatrix();
		Transform::Move(TranslateMatrix, Position.x, Position.y);

		if (GunType != "MG42") {
			// ���� ����
			if (LookDir == 0) {
				Transform::Rotate(TranslateMatrix, GunRotation + 180.0);
				Transform::Move(TranslateMatrix, -0.15 + RecoilPosition, 0.0);
				Flip(FLIP_H);
			}
			// ������ ����
			else if (LookDir == 1) {
				Transform::Rotate(TranslateMatrix, GunRotation);
				Transform::Move(TranslateMatrix, 0.15 - RecoilPosition, 0.0);
			}

			Transform::Scale(ScaleMatrix, 0.36, 0.36);

			if (GunType == "SCAR_H")
				Render(SCAR_H_Image);

			else if (GunType == "M16")
				Render(M16_Image);

			else if (GunType == "MP44")
				Render(MP44_Image);
		}

		else {
			// ���� ����
			if (LookDir == 0) {
				Transform::Rotate(TranslateMatrix, GunRotation + 180.0);
				Transform::Move(TranslateMatrix, -0.22 + RecoilPosition, 0.0);
				Flip(FLIP_H);
			}
			// ������ ����
			else if (LookDir == 1) {
				Transform::Rotate(TranslateMatrix, GunRotation);
				Transform::Move(TranslateMatrix, 0.22 - RecoilPosition, 0.0);
			}

			Transform::Scale(ScaleMatrix, 0.5, 0.45);
			Render(MG42_Image);
		}

		// �÷��̾� �г��� ǥ��
		Text.Render(Position.x, Position.y + 0.2, 0.1, 1.0, L"%ls", ToWstr(PlayerTag));
	}
};