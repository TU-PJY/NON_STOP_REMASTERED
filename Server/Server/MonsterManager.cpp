#include "MonsterManager.h"
#include "Packet.h"
#include "Container.h"
#include <chrono>
#include <random>
#include <iostream>

std::random_device rd;
std::uniform_int_distribution uid {0, 1};

std::vector<int> MonsterIDList;

float GenTimer;
float GenInterval = 2000.0;
float FrameTime;

int MonsterID = 1;

DWORD WINAPI MonsterThread(LPVOID lpParam) {
	auto previousTime = std::chrono::high_resolution_clock::now();

	while (true) {
		EnterCriticalSection(&ThreadSection);
		int LocalConnectedPlayer = ConnectedClients.size();
		LeaveCriticalSection(&ThreadSection);

		//std::chrono::duration<float> deltaTime{};

		if (LocalConnectedPlayer > 0) {
			//auto currentTime = std::chrono::high_resolution_clock::now();

			GenTimer += 0.1;

			if (GenTimer >= GenInterval) {
				std::cout << "Added Monster " << MonsterID << std::endl;

				int RandomDir = uid(rd);

				InputPacketInfo InputPackInfo{};
				InputPackInfo.PacketType = PACKET_TYPE_MONSTER_ADD;
				InputPackInfo.SCMonsterAddPack.AddDir = RandomDir;
				InputPackInfo.SCMonsterAddPack.ID = MonsterID;

				ClientPacketQueue.push(InputPackInfo);

				MonsterIDList.emplace_back(MonsterID);
				++MonsterID;

				GenTimer = 0.0;
			}

			//deltaTime = currentTime - previousTime;
			//previousTime = currentTime;
		}
	}

	return 0;
}