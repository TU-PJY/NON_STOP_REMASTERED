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
            //std::println("Send {}: {} to {}", PacketType, Info->ID, Other->ID);
            if (ReturnValue == SOCKET_ERROR)
                continue;

            ReturnValue = send(Other->ClientSocket, Packet, Size, 0);
            if (ReturnValue == SOCKET_ERROR)
                continue;
        }
    }
}

// ���� -> Ŭ���̾�Ʈ ť ������
DWORD WINAPI ClientQueueThread(LPVOID lpParam) {
    while (true) {
        // ť�� ������� ������ ť�� �ִ� ���Ҹ� �ϳ��� ������.
        while (!ClientPacketQueue.empty()) {
            InputPacketInfo InputPackInfo{};
            ClientPacketQueue.pop(InputPackInfo);

            switch (InputPackInfo.PacketType) {
            case PACKET_TYPE_ENTER:
                SendToOther(InputPackInfo.Client, (char*)&InputPackInfo.PacketType, (char*)&InputPackInfo.SCInfoPack, sizeof(SC_INFO_PACKET));
                break;

            case PACKET_TYPE_MOVE:
                SendToOther(InputPackInfo.Client, (char*)&InputPackInfo.PacketType, (char*)&InputPackInfo.SCMovePack, sizeof(SC_PLAYER_MOVE_PACKET));
                break;
            }
        }
    }

    return 0;
}
