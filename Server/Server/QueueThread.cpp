#include "QueueThread.h"
#include "Packet.h"
#include "Container.h"
#include <print>

ClientPacketInfo Q_PacketInfo; // 다른 클라이언트로 보낼 패킷 정보


// 패킷타입에 따라 다른 패킷을 클라이언트에 전송한다.
// 자신의 클라이언트를 제외한 나머지 클라이언트에 전송한다.
void SendToOther(const char* PacketType, const char* Packet, int Size) {
    int ReturnValue{};
    EnterCriticalSection(&ThreadSection);
    auto& Local = ConnectedClients;
    LeaveCriticalSection(&ThreadSection);

    for (auto const& Other : Local) {
        if (Other != Q_PacketInfo.Client) {
            ReturnValue = send(Other->ClientSocket, PacketType, sizeof(uint8_t), 0);
            if (ReturnValue == SOCKET_ERROR)
                continue;

            ReturnValue = send(Other->ClientSocket, Packet, Size, 0);
            if (ReturnValue == SOCKET_ERROR)
                continue;
        }
    }
}

// 서버 -> 클라이언트 큐 스레드
DWORD WINAPI ClientQueueThread(LPVOID lpParam) {
    while (true) {
        // 큐가 비어있지 않으면 큐에 있는 원소를 하나씩 꺼낸다.
        while (!ClientPacketQueue.empty()) {
            ClientPacketQueue.pop(Q_PacketInfo);
            switch (Q_PacketInfo.PacketType) {
            case PACKET_TYPE_LOBBY:
                SendToOther((char*)&Q_PacketInfo.PacketType, (char*)&Q_PacketInfo.SC_LobbyPacket, sizeof(SC_LOBBY_PACKET));
                break;
            }
        }
    }

    return 0;
}
