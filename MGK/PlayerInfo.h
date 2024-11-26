#pragma once
#include "GameObject.h"

// 로비 정보 객체(임시 테스트용)

class PlayerInfo : public GameObject {
public:
	bool ReadyState{};
	std::string PlayerTag{ "mata_0319" };

	TextUtil Text{};

	PlayerInfo() {
		Text.Init(L"맑은 고딕", FW_NORMAL);
		Text.SetColor(1.0, 1.0, 1.0);
		//Text.SetAlign(ALIGN_MIDDLE);
	}

	std::wstring ToWstr(const std::string& str) {
		int SizeNeed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
		std::wstring Wstr(SizeNeed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &Wstr[0], SizeNeed);

		return Wstr;
	}

	void InputKey(int State, unsigned char NormalKey, int SpecialKey) {
		if (State == NORMAL_KEY_DOWN) {
			switch (NormalKey) {
			case '1':
				PlayerGunType = "SCAR-H";  break;
			case '2':
				PlayerGunType = "M16";  break;
			case '3':
				PlayerGunType = "MP44";  break;
			case '4':
				PlayerGunType = "MG42";  break;
			case NK_SPACE:
				EX::SwitchBool(ReadyState); break;
			}
		}
	}

	void UpdateFunc() {

	}

	void RenderFunc() {
		// current state
		Text.Render(ASP(-0.8), 0.8, 0.3, 1.0, L"Lobby");
		Text.Render(ASP(-0.5), 0.6, 0.1, 1.0, L"My Tag: %ls", ToWstr(PlayerTag).c_str());
		Text.Render(ASP(-0.5), 0.5, 0.1, 1.0, L"Weapon: %ls", ToWstr(PlayerGunType).c_str());

		if (ReadyState)
			Text.Render(ASP(-0.5), 0.4, 0.1, 1.0, L"Ready");
		else
			Text.Render(ASP(-0.5), 0.4, 0.1, 1.0, L"Not Ready");
	}
};