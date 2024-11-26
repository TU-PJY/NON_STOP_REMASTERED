#include "TCP_Header.h"
#include "ClientThread.h"
#include "Packet.h"
#include "Container.h"

// 클라이언트 스레드
DWORD WINAPI ClientThread(LPVOID lpParam) {
    bool ConnectState = true; // 연결 상태, false가 될 시 스레드 즉시 종료
    int ReturnValue{};

    // 받은 패킷 타입
    uint8_t RecievePacketType{};

    /////////////// 클라이언트 소켓 및 주소
    SOCKET ClientSocket{}; // 클라이언트 소켓
    struct sockaddr_in ClientAddr {};  // 클라이언트 ip 주소

    // 접속한 클라 정보 받기
    ClientInfo* ThisClient = (ClientInfo*)lpParam;
    ClientSocket = ThisClient->ClientSocket;
    ClientAddr = ThisClient->ClientAddr;
    inet_ntop(AF_INET, &ClientAddr.sin_addr, ThisClient->Addr, sizeof(ThisClient->Addr));

    ClientPacketInfo C_PacketInfo{};
    CS_LOBBY_PACKET CS_LobbyPacket{};

    while (ConnectState) {
        // 클라이언트로부터 패킷 타입을 먼저 받는다
        ReturnValue = recv(ClientSocket, (char*)&RecievePacketType, sizeof(uint8_t), 0);
        if (ReturnValue == SOCKET_ERROR)
            break;

        // 받은 패킷 타입에 따라 다른 패킷 타입을 받는다
        switch (RecievePacketType) {
        case PACKET_TYPE_LOBBY:
            // 받기 전 구조체 초기화
            // CS_LobbyPacket 받기
            memset(&CS_LobbyPacket, 0, sizeof(CS_LobbyPacket));
            ReturnValue = recv(ClientSocket, (char*)&CS_LobbyPacket, sizeof(CS_LOBBY_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR) {
                ConnectState = false;
                break;
            }

            // 정보 저장 전 구조체 초기화
            // 플레이어 태그, 플레이어 총 타입, 플레이어 준비상태 데이터 복사
            memset(&C_PacketInfo, 0, sizeof(C_PacketInfo));
            strcpy(C_PacketInfo.SC_LobbyPacket.PlayerTag, CS_LobbyPacket.PlayerTag);
            strcpy(C_PacketInfo.SC_LobbyPacket.GunType, CS_LobbyPacket.GunType);
            C_PacketInfo.SC_LobbyPacket.ReadyState = CS_LobbyPacket.ReadyState;
            C_PacketInfo.PacketType = RecievePacketType;
            C_PacketInfo.Client = ThisClient;

            // 큐에 클라이언트 클라이언트로 보낼 패킷 정보 추가
            ClientPacketQueue.push(C_PacketInfo);
            break;

        case PACKET_TYPE_PLAYER_ADD:
            memset(&C_PacketInfo, 0, sizeof(C_PacketInfo));
            strcpy(C_PacketInfo.SC_LobbyPacket.PlayerTag, CS_LobbyPacket.PlayerTag);
            C_PacketInfo.PacketType = RecievePacketType;
            C_PacketInfo.Client = ThisClient;
            ClientPacketQueue.push(C_PacketInfo);
            break;
        }


    }

    // 접속 종료 시 접속한 클라이언트 목록에서 제거 후 소켓 닫기
    EnterCriticalSection(&ThreadSection);
    auto It = std::find(ConnectedClients.begin(), ConnectedClients.end(), ThisClient);
    if (It != ConnectedClients.end())
        ConnectedClients.erase(It);
    LeaveCriticalSection(&ThreadSection);

    delete ThisClient;
    closesocket(ClientSocket);

    return 0;
}