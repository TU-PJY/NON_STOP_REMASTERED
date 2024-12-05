#include "ClientThread.h"
#include "Packet.h"
#include "Scene.h"
#include "EngineHeader.h"

#include "OtherPlayer.h"

void Disconnect() {
    EnterCriticalSection(&ThreadSection);
    ConnectState = false;
    LeaveCriticalSection(&ThreadSection);
}

DWORD WINAPI ClientThread(LPVOID lpParam) {
    int ReturnValue{};
    int RecvPackType{};

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

    SC_INFO_PACKET SCInfoPack{};

    // 패킷 타입을 보낸다.
    int SendPacketType = PACKET_TYPE_ENTER;
    ReturnValue = send(ClientSocket, (char*)&SendPacketType, sizeof(uint8_t), 0);
    std::cout << "Send Packet:" << SendPacketType << "\n";
    if (ReturnValue == SOCKET_ERROR)
        err_quit("send() PACKET_TYPE_LOBBY");

    // 클라이언트 자신의 상태를 패킷에 복사
    CS_LOBBY_PACKET CSInfoPack{};
    EnterCriticalSection(&ThreadSection);
    strcpy(CSInfoPack.PlayerTag, PlayerTag.c_str());
    strcpy(CSInfoPack.GunType, PlayerGunType.c_str());
    LeaveCriticalSection(&ThreadSection);

    //  패킷 보내기
    ReturnValue = send(ClientSocket, (char*)&CSInfoPack, sizeof(CS_LOBBY_PACKET), 0);
    if (ReturnValue == SOCKET_ERROR)
        err_quit("send() CS_LOBBY_PACKET");

    while (ConnectState) {
        EnterCriticalSection(&ThreadSection);
        std::string LocalModeName = scene.Mode();
        LeaveCriticalSection(&ThreadSection);

        // 플레이 모드 패킷 타입 전송
        if (LocalModeName == "PlayMode") {
            // 움직임 패킷 전송
            int SendPacketType = PACKET_TYPE_MOVE;
            ReturnValue = send(ClientSocket, (char*)&SendPacketType, sizeof(uint8_t), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("recv() PakcetType");

            CS_PLAYER_MOVE_PACKET CSMovePack{};
            glm::vec2 RecvPosition{};

            EnterCriticalSection(&ThreadSection);
            if (auto Player = scene.Find("player"); Player) {
                strcpy(CSMovePack.PlayerTag, PlayerTag.c_str());
                RecvPosition = Player->GetPosition();
            }
            LeaveCriticalSection(&ThreadSection);

            CSMovePack.x = RecvPosition.x;
            CSMovePack.y = RecvPosition.y;

            ReturnValue = send(ClientSocket, (char*)&CSMovePack, sizeof(CS_PLAYER_MOVE_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("send() CS_LOBBY_PACKET");
        }

        // 패킷 타입 받기
        ReturnValue = recv(ClientSocket, (char*)&RecvPackType, sizeof(uint8_t), 0);
       // std::cout << "Recv Packet:" << RecvPackType << "\n";
        if (ReturnValue == SOCKET_ERROR)
            err_quit("recv() PakcetType");

        // 플레이어 입장
        if (RecvPackType == PACKET_TYPE_ENTER) {
            SC_INFO_PACKET SCInfoPack{};
            ReturnValue = recv(ClientSocket, (char*)&SCInfoPack, sizeof(SC_INFO_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("recv() SC_LOBBY_PACKET");

            EnterCriticalSection(&ThreadSection);

            auto it = std::find_if(ConnectedPlayer.begin(), ConnectedPlayer.end(),
                [&](const OTHER_CLIENT& client) {
                    return client.PlayerTag == std::string(SCInfoPack.PlayerTag);
                });

           if(it == ConnectedPlayer.end()) {
                // 새로운 클라이언트 추가
                OTHER_CLIENT newClient{};
                newClient.PlayerTag = std::string(SCInfoPack.PlayerTag);
                ConnectedPlayer.push_back(newClient);

                // 타 플레이어 객체 추가 후 이름과 총 타입 지정
                scene.AddObject(new OtherPlayer, SCInfoPack.PlayerTag, LAYER_3);
                if (auto Other = scene.Find(SCInfoPack.PlayerTag); Other) {
                    Other->SetPlayerTag(SCInfoPack.PlayerTag);
                    Other->SetGunType(SCInfoPack.GunType);
                    Other->SetPosition(glm::vec2(0.0, 0.0));

                    std::cout << SCInfoPack.PlayerTag << std::endl;

                }
            }

           LeaveCriticalSection(&ThreadSection);


            // 플레이어가 서버에 접속할 경우 타 클라이언트에게 자신이 있음을 알림
            int SendPacketType = PACKET_TYPE_ENTER;
            ReturnValue = send(ClientSocket, (char*)&SendPacketType, sizeof(uint8_t), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("send() CS_LOBBY_PACKET");

            CS_LOBBY_PACKET CSInfoPack{};
            strcpy(CSInfoPack.PlayerTag, PlayerTag.c_str());
            strcpy(CSInfoPack.GunType, PlayerGunType.c_str());

            ReturnValue = send(ClientSocket, (char*)&CSInfoPack, sizeof(CS_LOBBY_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("send() CS_LOBBY_PACKET");
        }

        // 플레이어 움직임
        else if (PACKET_TYPE_MOVE) {
            SC_PLAYER_MOVE_PACKET SCMovePack{};
            ReturnValue = recv(ClientSocket, (char*)&SCMovePack, sizeof(SC_PLAYER_MOVE_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("recv() SC_LOBBY_PACKET");

            CS_PLAYER_MOVE_PACKET CSMovePack{};

            EnterCriticalSection(&ThreadSection);
            if (auto Other = scene.Find(SCMovePack.PlayerTag); Other) {
                Other->SetPosition(glm::vec2(SCMovePack.x, SCMovePack.y));
            }
            LeaveCriticalSection(&ThreadSection);
        }
    }

    closesocket(ClientSocket);
    WSACleanup();
    DeleteCriticalSection(&ThreadSection);

    std::cout << "Thread end\n";
    return 0;
}