#include "MonsterManager.h"
#include "Packet.h"
#include "Container.h"
#include <chrono>
#include <random>
#include <iostream>

std::random_device rd;
std::uniform_int_distribution uid {0, 1};

float GenTimer;
float GenInterval = 3.0;
float DeltaTime;

int MonsterID = 1;

DWORD WINAPI MonsterThread(LPVOID lpParam) {
    // ���� �ð� ����
    auto previousTime = std::chrono::high_resolution_clock::now();

    while (true) {
        EnterCriticalSection(&ThreadSection);
        int LocalConnectedPlayer = ConnectedClients.size();
        LeaveCriticalSection(&ThreadSection);

        if (LocalConnectedPlayer > 1) {
            GenTimer += DeltaTime;

            if (GenTimer >= GenInterval) {
                std::uniform_int_distribution IndexUID{ 0, LocalConnectedPlayer - 1 };

                int RandomDir = uid(rd);
                int RandomIndex = IndexUID(rd);

                EnterCriticalSection(&ThreadSection);
                auto LocalNameList = NameList;
                LeaveCriticalSection(&ThreadSection);

                std::string TrackTag = LocalNameList[RandomIndex];

                InputPacketInfo InputPackInfo{};
                InputPackInfo.PacketType = PACKET_TYPE_MONSTER_ADD;
                InputPackInfo.SCMonsterAddPack.AddDir = RandomDir;
                InputPackInfo.SCMonsterAddPack.ID = MonsterID;
                strcpy(InputPackInfo.SCMonsterAddPack.TrackTag, TrackTag.c_str());

                ClientPacketQueue.push(InputPackInfo);

                ++MonsterID;

              //  std::cout << "Added Monster ID: " << MonsterID << " Dir: " << RandomDir << std::endl;
                GenTimer = 0.0;
            }
        }

        // ���� �ð� ����
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - previousTime;
        previousTime = currentTime; // ���� �ð��� ����

        // DeltaTime ��
        DeltaTime = deltaTime.count(); // DeltaTime�� �� ������ ��ȯ
    }

    return 0;
}