#pragma once
#include "GameObject.h"
#include "ClientThread.h"
#include "PlayMode.h"

// �κ� ���� ��ü(�ӽ� �׽�Ʈ��)

class PlayerInfo : public GameObject {
public:
	TextUtil Text{};

	PlayerInfo() {
		Text.Init(L"���� ���", FW_NORMAL);
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

	// 1, 2, 3, 4: ���� ����
	// z, x, c: �г��� ����
	// �����̽���: �غ� ���� ����
	// ����: ����
	void InputKey(int State, unsigned char NormalKey, int SpecialKey) {
		if (State == NORMAL_KEY_DOWN) {
			switch (NormalKey) {
			case '1':
				PlayerGunType = "SCAR_H"; break;
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
						scene.SwitchMode(PlayMode::Start);
					}
				}
				break;
			}
		}
	}

	void UpdateFunc() {
		
	}
	
	void RenderFunc() {
		// ���� ���� ǥ��
		Text.Render(ASP(-0.5), 0.6, 0.1, 1.0, L"My Tag: %ls", ToWstr(PlayerTag).c_str());
		Text.Render(ASP(-0.5), 0.5, 0.1, 1.0, L"Weapon: %ls", ToWstr(PlayerGunType).c_str());
	}
};