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
			Text.Init(L"맑은 고딕", FW_BOLD);
			Text.SetAlign(ALIGN_MIDDLE);
			Text.SetColor(0.0, 0.0, 0.0);
			Text.SetRenderType(RENDER_TYPE_DEFAULT);
			TextInit = true;
		}
	}

	void RenderFunc() {

		// 행렬 초기화, 렌더링 전 반드시 실행해야한다.
		InitMatrix();

		// 플레이어 위치 값만큼 이동
		Transform::Move(TranslateMatrix, Position);

		// 플레이어 출력 크기 조정
		Transform::Scale(ScaleMatrix, 0.3, 0.3);

		// 왼쪽을 볼 경우 
		if (LookDir == LOOK_LEFT)
			Flip(FLIP_H);

		// 렌더링에 필요한 데이터들을 쉐이더로 전달 후 최종 렌더링
		Render(PlayerImage);
		


		// 타 플레이어 총 출력
		InitMatrix();
		Transform::Move(TranslateMatrix, Position.x, Position.y);

		if (GunType != "MG42") {
			// 왼쪽 방향
			if (LookDir == 0) {
				Transform::Rotate(TranslateMatrix, GunRotation + 180.0);
				Transform::Move(TranslateMatrix, -0.15 + RecoilPosition, 0.0);
				Flip(FLIP_H);
			}
			// 오른쪽 방향
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
			// 왼쪽 방향
			if (LookDir == 0) {
				Transform::Rotate(TranslateMatrix, GunRotation + 180.0);
				Transform::Move(TranslateMatrix, -0.22 + RecoilPosition, 0.0);
				Flip(FLIP_H);
			}
			// 오른쪽 방향
			else if (LookDir == 1) {
				Transform::Rotate(TranslateMatrix, GunRotation);
				Transform::Move(TranslateMatrix, 0.22 - RecoilPosition, 0.0);
			}

			Transform::Scale(ScaleMatrix, 0.5, 0.45);
			Render(MG42_Image);
		}

		// 플레이어 닉네임 표시
		Text.Render(Position.x, Position.y + 0.2, 0.1, 1.0, L"%ls", ToWstr(PlayerTag));
	}
};