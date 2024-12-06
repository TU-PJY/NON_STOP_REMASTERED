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

	TimerUtil KickTimer{}; // 30초간 어떠한 변화도 없을 시 퇴장당하는 타이머

	LineBrush Line{}; // 체력 막대 그리기용

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
			Text.Init(L"맑은 고딕", FW_BOLD);
			Text.SetAlign(ALIGN_MIDDLE);
			Text.SetColor(1.0, 1.0, 1.0);
			Text.SetRenderType(RENDER_TYPE_DEFAULT);
			TextInit = true;
		}

		// 반동이 감지되면 총구화염을 추가한다.
		if (RecoilPosition > 0.03) {
			KickTimer.Reset();

			if (!AddFlame) {
				glm::vec2 FlamePosition{};
				if (GunType != "MG42") {
					FlamePosition.x = Position.x + cos(glm::radians(GunRotation)) * 0.35;
					FlamePosition.y = Position.y + sin(glm::radians(GunRotation)) * 0.35;

					// 격발 시 사운드 출력과 함께 불꽃 오브젝트를 추가한다
					scene.AddObject(new Flame(FlamePosition.x, FlamePosition.y, GunRotation), "flame", LAYER_3);
				}

				else {
					FlamePosition.x = Position.x + cos(glm::radians(GunRotation)) * 0.5;
					FlamePosition.y = Position.y + sin(glm::radians(GunRotation)) * 0.5;

					// 격발 시 사운드 출력과 함께 불꽃 오브젝트를 추가한다
					scene.AddObject(new Flame(FlamePosition.x, FlamePosition.y, GunRotation), "flame", LAYER_3);
				}
				AddFlame = true;
			}
		}

		else
			AddFlame = false;

		// 30초간 아무런 변화가 없을 시 플레이어가 없는 것으로 간주하고 scene에서 삭제한다.
		KickTimer.Update(FT);

		if (KickTimer.Sec() >= 30)
			scene.DeleteObject(this);

		// 조작이 감지되면 타이머가 초기화 된다.
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

		// 플레이어 체력 표시
		Line.SetColor(0.0, 0.0, 0.0);
		Line.Draw(Position.x - 0.15 - 0.025, Position.y + 0.2, Position.x + 0.15 + 0.025, Position.y + 0.2, 0.05, 1.0);
		Line.SetColor(1.0, 0.0, 0.0);
		Line.Draw(Position.x - 0.145 - 0.025, Position.y + 0.2, Position.x - 0.145 + (0.29 + 0.025) * ((float)HP / 100.0), Position.y + 0.2, 0.03, 1.0);

		// 플레이어 닉네임 표시
		Line.SetColor(0.0, 0.0, 0.0);
		Line.Draw(Position.x - 0.15, Position.y + 0.27, Position.x + 0.15, Position.y + 0.27, 0.1, 0.3);
		Text.Render(Position.x, Position.y + 0.25, 0.1, 1.0, L"%ls", ToWstr(PlayerTag));
	}
};