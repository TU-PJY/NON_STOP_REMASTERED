#include "QueueThread.h"
#include "Packet.h"

// 서버 -> 클라이언트 큐 스레드
DWORD WINAPI ClientQueueThread(LPVOID lpParam) {
    ClientPacketInfo PacketInfo{};
    int ReturnValue{};

    while (true) {
        // 큐가 비어있지 않으면 큐에 있는 원소를 하나씩 꺼낸다.
        while (!ClientPacketQueue.pop(PacketInfo)) {
            // 패킷타입에 따라 다른 패킷을 클라이언트에 전송한다.
            // 자신의 클라이언트를 제외한 나머지 클라이언트에 전송한다.
            switch (PacketInfo.PacketType) {
            case PACKET_TYPE_LOBBY:
                EnterCriticalSection(&ThreadSection);
                for (auto const& Other : ConnectedClients) {
                    if (Other != PacketInfo.Client) {
                        ReturnValue = send(Other->ClientSocket, (char*)&PacketInfo.PacketType, sizeof(uint8_t), 0);
                        if (ReturnValue == SOCKET_ERROR)
                            continue;

                        ReturnValue = send(Other->ClientSocket, (char*)static_cast<SC_LOBBY_PACKET*>(PacketInfo.PacketPtr), sizeof(SC_LOBBY_PACKET), 0);
                        if (ReturnValue == SOCKET_ERROR)
                            continue;
                    }
                }
                LeaveCriticalSection(&ThreadSection);

                // 가리키는 패킷 구조체 객체 삭제
                delete PacketInfo.PacketPtr;
                break;
            }
        }
    }

    return 0;
}
