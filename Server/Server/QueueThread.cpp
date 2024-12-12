#include "QueueThread.h"
#include "Packet.h"
#include "Container.h"
#include <print>


// ��ŶŸ�Կ� ���� �ٸ� ��Ŷ�� Ŭ���̾�Ʈ�� �����Ѵ�.
// �ڽ��� Ŭ���̾�Ʈ�� ������ ������ Ŭ���̾�Ʈ�� �����Ѵ�.
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

// ���� -> Ŭ���̾�Ʈ ť ������
DWORD WINAPI ClientQueueThread(LPVOID lpParam) {
    while (true) {
        // ť�� ������� ������ ť�� �ִ� ���Ҹ� �ϳ��� ������.
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
