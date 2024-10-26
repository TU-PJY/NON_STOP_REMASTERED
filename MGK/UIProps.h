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

class IndAmmo : public GameObject {
private:
	// 플레이어가 가지는 총의 최대 장탄수
	int AmmoLimit{};

	// 플레이어가 가지는 총의 현재 장탄수
	int CurrentAmmo{};
	int PrevAmmo{};

	// 장탄수 텍스트 높이
	GLfloat Height{};

	// 장탄수를 표시하기 위한 텍스트
	TextUtil text;

public:
	IndAmmo() {
		text.Init(L"맑은 고딕", FW_BOLD);
		PrevAmmo = CurrentAmmo;
	}

	// 현재 장탄수를 입력한다
	void InputCurrentAmmo(int Value) {
		CurrentAmmo = Value;
	}

	void UpdateFunc(float FT) {
		// 총알을 발사한 것이 확인되면 텍스트 애니메이션을 출력한다
		if (CurrentAmmo != PrevAmmo) {
			Height = 0.1;
			PrevAmmo = CurrentAmmo;
		}

		Height = Math::Lerp(Height, 0.0, 15, FT);
	}

	void RenderFunc() {
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
	}
};