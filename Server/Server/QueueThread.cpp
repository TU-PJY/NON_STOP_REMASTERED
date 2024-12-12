#include "QueueThread.h"
#include "Packet.h"
#include "Container.h"
#include <print>


// 패킷타입에 따라 다른 패킷을 클라이언트에 전송한다.
// 자신의 클라이언트를 제외한 나머지 클라이언트에 전송한다.
void SendToOther(ClientInfo* Info, const char* PacketType, const char* Packet, int Size) {
    int ReturnValue{};
    EnterCriticalSection(&ThreadSection);
    auto& Local = ConnectedClients;
    LeaveCriticalSection(&ThreadSection);

    for (auto const& Other : Local) {
        if (Other != Info) {
            ReturnValue = send(Other->ClientSocket, PacketType, sizeof(uint8_t), 0);
            if (ReturnValue == SOCKET_ERROR)
                continue;

            ReturnValue = send(Other->ClientSocket, Packet, Size, 0);
            if (ReturnValue == SOCKET_ERROR)
                continue;
        }
    }
}

void SendToAll(const char* PacketType, const char* Packet, int Size) {
    int ReturnValue{};
    EnterCriticalSection(&ThreadSection);
    auto& Local = ConnectedClients;
    LeaveCriticalSection(&ThreadSection);

    for (auto const& Client : Local) {
        ReturnValue = send(Client->ClientSocket, PacketType, sizeof(uint8_t), 0);
        if (ReturnValue == SOCKET_ERROR)
            continue;

        ReturnValue = send(Client->ClientSocket, Packet, Size, 0);
        if (ReturnValue == SOCKET_ERROR)
            continue;
    }
}

void SendToMe(ClientInfo* Info, const char* PacketType, const char* Packet, int Size) {
    int ReturnValue{};
    EnterCriticalSection(&ThreadSection);
    auto& Local = ConnectedClients;
    LeaveCriticalSection(&ThreadSection);

    for (auto const& Client : Local) {
        if (Client == Info) {
            ReturnValue = send(Client->ClientSocket, PacketType, sizeof(uint8_t), 0);
            if (ReturnValue == SOCKET_ERROR)
                continue;

            ReturnValue = send(Client->ClientSocket, Packet, Size, 0);
            if (ReturnValue == SOCKET_ERROR)
                continue;
        }
    }
}

// 서버 -> 클라이언트 큐 스레드
DWORD WINAPI ClientQueueThread(LPVOID lpParam) {
    while (true) {
        // 큐가 비어있지 않으면 큐에 있는 원소를 하나씩 꺼낸다.
        if (!ClientPacketQueue.empty()) {
            InputPacketInfo InputPackInfo{};
            ClientPacketQueue.pop(InputPackInfo);

            switch (InputPackInfo.PacketType) {
            case PACKET_TYPE_ENTER:
                SendToOther(InputPackInfo.Client, (char*)&InputPackInfo.PacketType, (char*)&InputPackInfo.SCInfoPack, sizeof(SC_INFO_PACKET));
                break;

            case PACKET_TYPE_PLAYER:
                SendToOther(InputPackInfo.Client, (char*)&InputPackInfo.PacketType, (char*)&InputPackInfo.SCPlayerPack, sizeof(SC_PLAYER_PACKET));
                break;

            case PACKET_TYPE_MONSTER_ADD:
                SendToOther(InputPackInfo.Client, (char*)&InputPackInfo.PacketType, (char*)&InputPackInfo.SCMonsterAddPack, sizeof(SC_MONSTER_ADD_PACKET));
                break;

            case PACKET_TYPE_MONSTER_DAMAGE:
                SendToOther(InputPackInfo.Client, (char*)&InputPackInfo.PacketType, (char*)&InputPackInfo.SCMonsterDamagePack, sizeof(SC_MONSTER_DAMAGE_PACKET));
                break;

            case PACKET_TYPE_MONSTER_DELETE:
                SendToOther(InputPackInfo.Client, (char*)&InputPackInfo.PacketType, (char*)&InputPackInfo.SCMonsterDeletePack, sizeof(SC_MONSTER_DELETE_PACKET));
                break;

            }
        }
    }

    return 0;
}
