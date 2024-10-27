#pragma once
#include "GameObject.h"

// 장탄수 인디케이터 뒷 배경
class IndBack : public GameObject {
public:
	void RenderFunc() {
		InitMatrix(RENDER_TYPE_STATIC);
		Transform::Move(TranslateMatrix, ASP(1.0) - 0.5, -1.5 + 0.5);
		Render(IndicatorBackImage);
	}
};


// 장탄수 인디케이터
class IndAmmo : public GameObject {
private:
	// 플레이어가 가지는 총의 최대 장탄수
	int AmmoLimit{};

	// 플레이어가 가지는 총의 현재 장탄수
	int CurrentAmmo{};

	// 인디케이터가 기억하는 이전 장탄수
	int PrevAmmo{};
	
	// 플레이어 총 재장전 상태
	bool ReloadState{};

	// 재장전 진행도 수치
	GLfloat ReloadProgress{}, EntireReloadProgress{};

	// 장탄수 텍스트 높이
	GLfloat Height{};

	// 장탄수를 표시하기 위한 텍스트
	TextUtil text;

	// 재장전을 표시하기 위한 선
	LineBrush line;

public:
	IndAmmo() {
		text.Init(L"맑은 고딕", FW_BOLD);
		line.SetRenderType(RENDER_TYPE_STATIC);
		line.SetColor(1.0, 1.0, 1.0);
	}

	// 현재 장탄수를 입력한다
	void InputCurrentAmmo(int Value) {
		CurrentAmmo = Value;
	}

	// 재장전 상태를 입력한다
	void InputReloadState(bool State) {
		ReloadState = State;
	}

	// 재장전 진척도를 입력한다
	void InputReloadProgress(GLfloat CurrentProgress, GLfloat EntireProgress) {
		EntireReloadProgress = EntireProgress;
		ReloadProgress = CurrentProgress;
	}

	void UpdateFunc(float FT) {
		// 장탄수 업데이트가 감지되면 텍스트 애니메이션을 출력한다
		if (CurrentAmmo != PrevAmmo) {
			Height = 0.1;
			PrevAmmo = CurrentAmmo;
		}

		Height = Math::Lerp(Height, 0.0, 15, FT);
	}

	void RenderFunc() {
		// 장탄수가 1발 이상 있다면 장탄수를 표시하고, 장탄수가 0이라면 R을 표시한다.
		if (CurrentAmmo > 0) {
			text.SetColorRGB(255, 204, 0);
			text.Render(ASP(1.0) - 0.4, -1.0 + 0.05 + Height, 0.25, 1.0, L"%d", CurrentAmmo);
		}
		else {
			text.SetColor(1.0, 0.0, 0.0);
			text.Render(ASP(1.0) - 0.4, -1.0 + 0.05 + Height, 0.25, 1.0, L"R");
		}

		// 현재 사용중인 총 표시
		InitMatrix(RENDER_TYPE_STATIC);
		SetColor(1.0, 1.0, 1.0);
		Transform::Move(TranslateMatrix, ASP(1.0) - 0.6, -1.0 + 0.13);
		Transform::Scale(ScaleMatrix, 0.3, 0.3);

		if(PlayerGunName == "SCAR_H")
			Render(SCAR_H_Image);

		// 재장전 진행도 막대 표시
		if (ReloadState) {
			GLfloat PositionX = ASP(1.0) - 0.8;
			line.Draw(PositionX, -1.0 + 0.22, PositionX + 0.7 * (ReloadProgress / EntireReloadProgress), -1.0 + 0.22, 0.02);
		}
	}
};

// 수류탄 인디케이터
class IndGrenade : public GameObject {
private:
	bool UseEnable{};
	int RemainedTime{};

public:
	void UpdateFunc(float FT) {

	}

	void RenderFunc() {

	}
};

// 체력 인디케이터
class IndHP : public GameObject {
private:
	// 플레이어 현재 체력
	int CurrentHP{};

	// 인디케이터가 기억하는 이전 체력
	int PrevHP{};

	// 체력 바 표시용 선
	LineBrush line;

	// 체력 표시용 텍스트
	TextUtil text;

public:
	IndHP() {
		text.Init(L"맑은 고딕", FW_BOLD);
		text.SetColor(1.0, 1.0, 1.0);
		line.SetRenderType(RENDER_TYPE_STATIC);
	}

	// 플레이어의 현재 HP를 입력한다.
	void InputCurrentHP(int Value) {
		CurrentHP = Value;
	}

	void UpdateFunc(float FT) {
		if (CurrentHP != PrevHP) {
			
		}
	}

	void RenderFunc() {
		GLfloat PositionX = ASP(-1.0) + 0.1;

		line.SetColor(0.0, 0.0, 0.0);
		line.Draw(PositionX, -1.0 + 0.1, PositionX + 0.7, -1.0 + 0.1, 0.1);

		line.SetColor(1.0, 0.0, 0.0);
		line.Draw(PositionX, -1.0 + 0.1, PositionX + 0.7 * ( (float)CurrentHP / 100.0 ), -1.0 + 0.1, 0.08);

		text.Render(ASP(-1.0) + 0.07, -1.0 + 0.075, 0.1, 1.0, L"%d", CurrentHP);
	}
};