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
float GenInterval = 1.0;
float DeltaTime;

int MonsterID = 1;

DWORD WINAPI MonsterThread(LPVOID lpParam) {
    // 이전 시간 저장
    auto previousTime = std::chrono::high_resolution_clock::now();

    while (true) {
        // 현재 시간 측정
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - previousTime;
        previousTime = currentTime; // 이전 시간을 갱신

        // DeltaTime 값
        float DeltaTime = deltaTime.count(); // DeltaTime을 초 단위로 변환

        EnterCriticalSection(&ThreadSection);
        int LocalConnectedPlayer = ConnectedClients.size();
        LeaveCriticalSection(&ThreadSection);

        if (LocalConnectedPlayer > 1) {
            GenTimer += DeltaTime;

            if (GenTimer >= GenInterval) {

                int RandomDir = uid(rd);

                InputPacketInfo InputPackInfo{};
                InputPackInfo.PacketType = PACKET_TYPE_MONSTER_ADD;
                InputPackInfo.SCMonsterAddPack.AddDir = RandomDir;
                InputPackInfo.SCMonsterAddPack.ID = MonsterID;

                ClientPacketQueue.push(InputPackInfo);

                MonsterIDList.emplace_back(MonsterID);
                ++MonsterID;

                std::cout << "Added Monster ID: " << MonsterID << " Dir: " << RandomDir << std::endl;
                GenTimer = 0.0;
            }
        }
    }

    return 0;
}