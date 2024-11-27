#pragma once
#include "GameObject.h"
#include "ClientThread.h"

// 로비 정보 객체(임시 테스트용)

class PlayerInfo : public GameObject {
public:
	TextUtil Text{};

	PlayerInfo() {
		Text.Init(L"맑은 고딕", FW_NORMAL);
		Text.SetColor(1.0, 1.0, 1.0);

		PlayerTag = "";
		PlayerGunType = "";
		PlayerReadyState = false;
	}

	std::wstring ToWstr(const std::string& str) {
		int SizeNeed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
		std::wstring Wstr(SizeNeed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &Wstr[0], SizeNeed);

		return Wstr;
	}

	// 1, 2, 3, 4: 무기 선택
	// z, x, c: 닉네임 변경
	// 스페이스바: 준비 상태 변경
	// 엔터: 접속
	void InputKey(int State, unsigned char NormalKey, int SpecialKey) {
		if (State == NORMAL_KEY_DOWN) {
			switch (NormalKey) {
			case '1':
				PlayerGunType = "SCAR-H"; break;
			case '2':
				PlayerGunType = "M16"; break;
			case '3':
				PlayerGunType = "MP44"; break;
			case '4':
				PlayerGunType = "MG42"; break;

			case 'z':
				PlayerTag = "Player1"; break;
			case 'x':
				PlayerTag = "Player2"; break;
			case 'c':
				PlayerTag = "Player3"; break;

			case NK_SPACE:
				EX::SwitchBool(PlayerReadyState); 
				break;

			case NK_ENTER:
				if (!ConnectState) {
					HANDLE Thread = CreateThread(NULL, 0, ClientThread, NULL, 0, 0);
					if (Thread) {
						CloseHandle(Thread);
						ConnectState = true;
					}
				}
				break;
			}
		}
	}

	void UpdateFunc() {
		
	}
	
	void RenderFunc() {
		// 나의 상태 표시
		Text.Render(ASP(-0.5), 0.6, 0.1, 1.0, L"My Tag: %ls", ToWstr(PlayerTag).c_str());
		Text.Render(ASP(-0.5), 0.5, 0.1, 1.0, L"Weapon: %ls", ToWstr(PlayerGunType).c_str());

		if (PlayerReadyState)
			Text.Render(ASP(-0.5), 0.4, 0.1, 1.0, L"Ready");
		else
			Text.Render(ASP(-0.5), 0.4, 0.1, 1.0, L"Not Ready");

		// 연결된 상태라면 타 클라이언트 상태 표시
		if(ConnectState) {
			// current state
			Text.Render(ASP(-0.8), 0.8, 0.3, 1.0, L"Connected");

			// 다른 플레이어들 상태 표시
			GLfloat HeightOffset{-0.15};
			int PlayerNum = 1;
			for (auto& Other : ConnectedPlayer) {
				GLfloat Height = 0.25 + HeightOffset;
				Text.Render(ASP(-0.5), Height, 0.1, 1.0, L"Player %d: %ls", PlayerNum, ToWstr(Other.PlayerTag).c_str());
				Text.Render(ASP(-0.5), Height - 0.1, 0.1, 1.0, L"Weapon: %ls", ToWstr(Other.GunType).c_str());

				if (Other.ReadyState)
					Text.Render(ASP(-0.5), Height - 0.2, 0.1, 1.0, L"Ready");
				else
					Text.Render(ASP(-0.5), Height - 0.2, 0.1, 1.0, L"Not Ready");

				HeightOffset -= 0.5;
				++PlayerNum;
			}
		}

		// 연결이 안되어 있다면 연결이 안되어 있음을 표시
		else 
			Text.Render(ASP(-0.8), 0.8, 0.3, 1.0, L"Not connected");
	}
};