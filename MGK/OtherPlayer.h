#pragma once
#include "GameObject.h"
#include "TextUtil.h"
#include "Flame.h"

enum EnumLookDir {
	LOOK_LEFT,
	LOOK_RIGHT
};

class OtherPlayer : public GameObject {
public:
	glm::vec2 Position{};
	glm::vec2 PrevPosition{};

	std::string GunType{};
	std::string Tag{};
	GLfloat GunRotation{};
	GLfloat PrevGunRotation{};

	int HP{};

	GLfloat RecoilPosition{};
	int LookDir{};
	bool AddFlame{};

	bool TextInit{};
	TextUtil Text{};

	TimerUtil KickTimer{}; // 30�ʰ� ��� ��ȭ�� ���� �� ������ϴ� Ÿ�̸�

	LineBrush Line{}; // ü�� ���� �׸����

	int Ref{};

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

	void SetHP(int Value) {
		HP = Value;
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
			Text.SetColor(1.0, 1.0, 1.0);
			Text.SetRenderType(RENDER_TYPE_DEFAULT);
			TextInit = true;
		}

		// �ݵ��� �����Ǹ� �ѱ�ȭ���� �߰��Ѵ�.
		if (RecoilPosition > 0.03) {
			KickTimer.Reset();

			if (!AddFlame) {
				glm::vec2 FlamePosition{};
				if (GunType != "MG42") {
					FlamePosition.x = Position.x + cos(glm::radians(GunRotation)) * 0.35;
					FlamePosition.y = Position.y + sin(glm::radians(GunRotation)) * 0.35;

					// �ݹ� �� ���� ��°� �Բ� �Ҳ� ������Ʈ�� �߰��Ѵ�
					scene.AddObject(new Flame(FlamePosition.x, FlamePosition.y, GunRotation), "flame", LAYER_3);
				}

				else {
					FlamePosition.x = Position.x + cos(glm::radians(GunRotation)) * 0.5;
					FlamePosition.y = Position.y + sin(glm::radians(GunRotation)) * 0.5;

					// �ݹ� �� ���� ��°� �Բ� �Ҳ� ������Ʈ�� �߰��Ѵ�
					scene.AddObject(new Flame(FlamePosition.x, FlamePosition.y, GunRotation), "flame", LAYER_3);
				}
				AddFlame = true;
			}
		}

		else
			AddFlame = false;

		// 30�ʰ� �ƹ��� ��ȭ�� ���� �� �÷��̾ ���� ������ �����ϰ� scene���� �����Ѵ�.
		KickTimer.Update(FT);

		if (KickTimer.Sec() >= 30)
			scene.DeleteObject(this);

		// ������ �����Ǹ� Ÿ�̸Ӱ� �ʱ�ȭ �ȴ�.
		if (PrevPosition != Position) {
			PrevPosition = Position;
			KickTimer.Reset();
		}

		if (PrevGunRotation != GunRotation) {
			PrevGunRotation = GunRotation;
			KickTimer.Reset();
		}
	}

	void RenderFunc() {
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

		// �÷��̾� ü�� ǥ��
		Line.SetColor(0.0, 0.0, 0.0);
		Line.Draw(Position.x - 0.15 - 0.025, Position.y + 0.2, Position.x + 0.15 + 0.025, Position.y + 0.2, 0.05, 1.0);
		Line.SetColor(1.0, 0.0, 0.0);
		Line.Draw(Position.x - 0.145 - 0.025, Position.y + 0.2, Position.x - 0.145 + (0.29 + 0.025) * ((float)HP / 100.0), Position.y + 0.2, 0.03, 1.0);

		// �÷��̾� �г��� ǥ��
		Line.SetColor(0.0, 0.0, 0.0);
		Line.Draw(Position.x - 0.15, Position.y + 0.27, Position.x + 0.15, Position.y + 0.27, 0.1, 0.3);
		Text.Render(Position.x, Position.y + 0.25, 0.1, 1.0, L"%ls", ToWstr(PlayerTag));
	}
};