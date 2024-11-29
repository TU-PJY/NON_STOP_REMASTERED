#include "ClientThread.h"
#include "Packet.h"
#include "Scene.h"
#include "EngineHeader.h"

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

    SC_LOBBY_PACKET SC_LobbyPacket{};

    // 패킷 타입을 보낸다.
    int SendPacketType = PACKET_TYPE_LOBBY;
    ReturnValue = send(ClientSocket, (char*)&SendPacketType, sizeof(uint8_t), 0);
    std::cout << "Send Packet:" << SendPacketType << "\n";
    if (ReturnValue == SOCKET_ERROR)
        err_quit("send() PACKET_TYPE_LOBBY");

    // 클라이언트 자신의 상태를 패킷에 복사
    CS_LOBBY_PACKET CS_LobbyPacket{};
    EnterCriticalSection(&ThreadSection);
    strcpy(CS_LobbyPacket.PlayerTag, PlayerTag.c_str());
    strcpy(CS_LobbyPacket.GunType, PlayerGunType.c_str());
    CS_LobbyPacket.ReadyState = PlayerReadyState;
    LeaveCriticalSection(&ThreadSection);

    //  패킷 보내기
    ReturnValue = send(ClientSocket, (char*)&CS_LobbyPacket, sizeof(CS_LOBBY_PACKET), 0);
    if (ReturnValue == SOCKET_ERROR)
        err_quit("send() CS_LOBBY_PACKET");

    while (ConnectState) {
        // 움직임 패킷 전송
        {
            int SendPacketType = PACKET_TYPE_MOVE;
            ReturnValue = send(ClientSocket, (char*)&SendPacketType, sizeof(uint8_t), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("recv() PakcetType");

            CS_PLAYER_MOVE_PACKET CS_MovePacket{};
            if (auto Player = scene.Find("player"); Player) {
                strcpy(CS_MovePacket.PlayerTag, PlayerTag.c_str());
                glm::vec2 Position = Player->GetPosition();
                CS_MovePacket.x = Position.x;
                CS_MovePacket.y = Position.y;
            }

            ReturnValue = send(ClientSocket, (char*)&CS_MovePacket, sizeof(CS_PLAYER_MOVE_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("send() CS_LOBBY_PACKET");
        }

        // 패킷 타입 받기
        ReturnValue = recv(ClientSocket, (char*)&RecievePacketType, sizeof(uint8_t), 0);
        std::cout << "Recv Packet:" << RecievePacketType << "\n";
        if (ReturnValue == SOCKET_ERROR)
            err_quit("recv() PakcetType");

        // 받은 패킷 처리
        switch (RecievePacketType) {
            // 타 클라이언트 정보 패킷
        case PACKET_TYPE_LOBBY:
        {
            SC_LOBBY_PACKET SC_LobbyPacket{};
            ReturnValue = recv(ClientSocket, (char*)&SC_LobbyPacket, sizeof(SC_LOBBY_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("recv() SC_LOBBY_PACKET");

            EnterCriticalSection(&ThreadSection);

            // 클라이언트가 이미 목록에 있는지 확인
            auto it = std::find_if(ConnectedPlayer.begin(), ConnectedPlayer.end(),
                [&](const OTHER_CLIENT& client) {
                    return client.PlayerTag == std::string(SC_LobbyPacket.PlayerTag);
                });

            if (it != ConnectedPlayer.end()) {
                // 기존 클라이언트 정보 업데이트
                it->GunType = std::string(SC_LobbyPacket.GunType);
                it->ReadyState = SC_LobbyPacket.ReadyState;
            }
            else {
                // 새로운 클라이언트 추가
                OTHER_CLIENT newClient{};
                newClient.PlayerTag = std::string(SC_LobbyPacket.PlayerTag);
                newClient.GunType = std::string(SC_LobbyPacket.GunType);
                newClient.ReadyState = SC_LobbyPacket.ReadyState;
                ConnectedPlayer.push_back(newClient);
            }

            LeaveCriticalSection(&ThreadSection);
        }
            break;

        case PACKET_TYPE_MOVE:
        {
            SC_PLAYER_MOVE_PACKET SC_MovePacket{};
            ReturnValue = recv(ClientSocket, (char*)&SC_MovePacket, sizeof(SC_PLAYER_MOVE_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("recv() SC_LOBBY_PACKET");

            std::cout << "Tag: " << SC_MovePacket.PlayerTag << "\n X: " << SC_MovePacket.x << "\n y:" << SC_MovePacket.y << "\n";
        }

            break;
        }
            
    }

    closesocket(ClientSocket);
    WSACleanup();
    DeleteCriticalSection(&ThreadSection);

    std::cout << "Thread end\n";
    return 0;
}