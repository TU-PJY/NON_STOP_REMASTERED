#include "ClientThread.h"
#include "Packet.h"
#include "Scene.h"

void Disconnect() {
    EnterCriticalSection(&ThreadSection);
    ConnectState = false;
    LeaveCriticalSection(&ThreadSection);
}

DWORD WINAPI ClientThread(LPVOID lpParam) {
    int ReturnValue{};
    int RecievePacketType{};

    // 윈속 초기화
    WSADATA WSA;
    if (WSAStartup(MAKEWORD(2, 2), &WSA) != 0)
        return 1;

    // 클라이언트 소켓 생성
    SOCKET ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ClientSocket == INVALID_SOCKET)
        err_quit("socket()");

    // connect()
    struct sockaddr_in ServerAddr;
    memset(&ServerAddr, 0, sizeof(ServerAddr));
    ServerAddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &ServerAddr.sin_addr);
    ServerAddr.sin_port = htons(SERVER_PORT);
    ReturnValue = connect(ClientSocket, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
    if (ReturnValue == SOCKET_ERROR)
        err_quit("connect()");

    InitializeCriticalSection(&ThreadSection);

    // 연결 상태 활성화
    EnterCriticalSection(&ThreadSection);
    ConnectState = true;
    LeaveCriticalSection(&ThreadSection);

    CS_LOBBY_PACKET CS_LobbyPacket{};

    // 서버 접속 시 서버에 접속 신호를 보낸다.
    int SendPacketType = PACKET_TYPE_PLAYER_ADD;
    ReturnValue = send(ClientSocket, (char*)&SendPacketType, sizeof(uint8_t), 0);
    if (ReturnValue == SOCKET_ERROR)
        Disconnect();

    while (ConnectState) {
        // 패킷을 받는다.
        ReturnValue = recv(ClientSocket, (char*)&RecievePacketType, sizeof(uint8_t), 0);
        if (ReturnValue == SOCKET_ERROR) 
            Disconnect();

        EnterCriticalSection(&ThreadSection);
        std::string CurrentMode = scene.Mode();
        LeaveCriticalSection(&ThreadSection);

        // 로비모드
        if (CurrentMode == "LobbyMode") {
            // 패킷 타입을 보낸다.
            int SendPacketType = PACKET_TYPE_LOBBY;
            ReturnValue = send(ClientSocket, (char*)&SendPacketType, sizeof(uint8_t), 0);
            if (ReturnValue == SOCKET_ERROR) 
                Disconnect();

            // 클라이언트 자신의 상태를 패킷에 복사
            memset(&CS_LobbyPacket, 0, sizeof(CS_LOBBY_PACKET));
            strcpy(CS_LobbyPacket.PlayerTag, PlayerTag.c_str());
            strcpy(CS_LobbyPacket.GunType, PlayerGunType.c_str());
            CS_LobbyPacket.ReadyState = PlayerReadyState;

            //  패킷 보내기
            ReturnValue = send(ClientSocket, (char*)&CS_LobbyPacket, sizeof(CS_LOBBY_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR) 
                Disconnect();

            // 받은 패킷 처리
            switch (RecievePacketType) {
                // 타 클라이언트 정보 패킷
            case PACKET_TYPE_LOBBY:
                SC_LOBBY_PACKET SC_LobbyPacket{};
                ReturnValue = recv(ClientSocket, (char*)&SC_LobbyPacket, sizeof(SC_LOBBY_PACKET), 0);
                if (ReturnValue == SOCKET_ERROR)
                    Disconnect();

                // 접속한 플레이어들의 정보를 최신화 한다.
                EnterCriticalSection(&ThreadSection);
                auto It = std::find_if(begin(ConnectedPlayer), end(ConnectedPlayer), [&](const OtherClient& Other) {return Other.PlayerTag == SC_LobbyPacket.PlayerTag; });
                if (It != end(ConnectedPlayer)) {
                    It->GunType = SC_LobbyPacket.GunType;
                    It->ReadyState = SC_LobbyPacket.ReadyState;
                }
                LeaveCriticalSection(&ThreadSection);
            break;

            // 로비 플레이어 추가 패킷
            case PACKET_TYPE_PLAYER_ADD:
                SC_LOBBY_PACKET SC_LobbyPacket{};
                ReturnValue = recv(ClientSocket, (char*)&SC_LobbyPacket, sizeof(SC_LOBBY_PACKET), 0);
                if (ReturnValue == SOCKET_ERROR)
                    Disconnect();

                OtherClient Other{};
                Other.PlayerTag = SC_LobbyPacket.PlayerTag;

                // 접속한 플레이어 리스트에 새 구조체를 추가한다.
                EnterCriticalSection(&ThreadSection);
                ConnectedPlayer.push_back(Other);
                ++NumPlayerConnected;
                LeaveCriticalSection(&ThreadSection);
                break;
            }
        }
    }

    closesocket(ClientSocket);
    WSACleanup();
    DeleteCriticalSection(&ThreadSection);
    return 0;
}