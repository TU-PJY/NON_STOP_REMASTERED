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

std::set<std::string> DuplicatedTag;

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

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

    SC_INFO_PACKET SCInfoPack{};

    // 패킷 타입을 보낸다.
    int SendPacketType = PACKET_TYPE_ENTER;
    ReturnValue = send(ClientSocket, (char*)&SendPacketType, sizeof(uint8_t), 0);
    std::cout << "Send Packet:" << SendPacketType << "\n";
    if (ReturnValue == SOCKET_ERROR)
        err_quit("send() PACKET_TYPE_LOBBY");

    // 클라이언트 자신의 상태를 패킷에 복사
    CS_INFO_PACKET CSInfoPack{};
    EnterCriticalSection(&ThreadSection);
    strcpy(CSInfoPack.PlayerTag, PlayerTag.c_str());
    strcpy(CSInfoPack.GunType, PlayerGunType.c_str());
    LeaveCriticalSection(&ThreadSection);

    //  패킷 보내기
    ReturnValue = send(ClientSocket, (char*)&CSInfoPack, sizeof(CS_INFO_PACKET), 0);
    if (ReturnValue == SOCKET_ERROR)
        err_quit("send() CS_LOBBY_PACKET");

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

    while (ConnectState) {
        EnterCriticalSection(&ThreadSection);
        bool LocalConnectState = ConnectState;
        std::string LocalModeName = scene.Mode();
        LeaveCriticalSection(&ThreadSection);

        // 연결 상태 해제 시 서버 스레드 종료
        if (!LocalConnectState)
            break;

        // 플레이 모드 패킷 타입 전송
        if (LocalModeName == "PlayMode") {
            // 움직임 패킷 전송
            int SendPacketType = PACKET_TYPE_PLAYER;
            ReturnValue = send(ClientSocket, (char*)&SendPacketType, sizeof(uint8_t), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("recv() PakcetType");

            CS_PLAYER_PACKET CSPlayerPack{};
            glm::vec2 SendPosition{};
            int SendLookDir{};
            GLfloat SendGunRotation{};
            GLfloat SendRecoilPosition{};
            int SendHP{};
            
            // 자신의 정보를 타 클라이언트에 전달한다
            EnterCriticalSection(&ThreadSection);
            if (auto Player = scene.Find("player"); Player) {
                strcpy(CSPlayerPack.PlayerTag, PlayerTag.c_str());
                SendPosition = Player->GetPosition();
                SendLookDir = Player->GetLookDir();
                SendGunRotation = Player->GetGunRotation();
                SendRecoilPosition = Player->GetRecoilPosition();
                SendHP = Player->GetHP();
            }
            LeaveCriticalSection(&ThreadSection);

            CSPlayerPack.x = SendPosition.x;
            CSPlayerPack.y = SendPosition.y;
            CSPlayerPack.LookDir = SendLookDir;
            CSPlayerPack.GunRotation = SendGunRotation;
            CSPlayerPack.RecoilPosition = SendRecoilPosition;
            CSPlayerPack.HP = SendHP;

            ReturnValue = send(ClientSocket, (char*)&CSPlayerPack, sizeof(CS_PLAYER_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("send() CS_LOBBY_PACKET");
        }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

        // 패킷 타입 받기
        ReturnValue = recv(ClientSocket, (char*)&RecvPackType, sizeof(uint8_t), 0);
        if (ReturnValue == SOCKET_ERROR)
            err_quit("recv() PakcetType");

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

        // 플레이어 입장
        if (RecvPackType == PACKET_TYPE_ENTER) {
            SC_INFO_PACKET SCInfoPack{};
            ReturnValue = recv(ClientSocket, (char*)&SCInfoPack, sizeof(SC_INFO_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("recv() SC_LOBBY_PACKET");

            EnterCriticalSection(&ThreadSection);

            // 중복되는 닉네임이 없을 경우 새로운 플레이어 추가
            auto It = std::find(begin(ConnectedPlayer), end(ConnectedPlayer), std::string(SCInfoPack.PlayerTag));

           if(It == ConnectedPlayer.end()) {
                // 새로운 닉네임 리스트 추가
                ConnectedPlayer.push_back(std::string(SCInfoPack.PlayerTag));

                // 타 플레이어 객체 추가 후 이름과 총 타입 지정
                scene.AddObject(new OtherPlayer, SCInfoPack.PlayerTag, LAYER_3);
                if (auto Other = scene.Find(SCInfoPack.PlayerTag); Other) {
                    Other->SetPlayerTag(SCInfoPack.PlayerTag);
                    Other->SetGunType(SCInfoPack.GunType);
                }
            }

           LeaveCriticalSection(&ThreadSection);

            // 타 클라이언트가 서버에 접속할 경우 타 클라이언트에게 자신이 있음을 알림
            int SendPacketType = PACKET_TYPE_ENTER;
            ReturnValue = send(ClientSocket, (char*)&SendPacketType, sizeof(uint8_t), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("send() CS_LOBBY_PACKET");

            CS_INFO_PACKET CSInfoPack{};
            strcpy(CSInfoPack.PlayerTag, PlayerTag.c_str());
            strcpy(CSInfoPack.GunType, PlayerGunType.c_str());

            ReturnValue = send(ClientSocket, (char*)&CSInfoPack, sizeof(CS_INFO_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("send() CS_LOBBY_PACKET");
        }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

        // 플레이어 움직임
        else if (PACKET_TYPE_PLAYER) {
            SC_PLAYER_PACKET SCPlayerPack{};
            ReturnValue = recv(ClientSocket, (char*)&SCPlayerPack, sizeof(SC_PLAYER_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("recv() SC_LOBBY_PACKET");

            CS_PLAYER_PACKET CSPlayerPack{};

            // 타 클라이언트 플레이어 객체 업데이트
            EnterCriticalSection(&ThreadSection);
            if (auto Other = scene.Find(SCPlayerPack.PlayerTag); Other) {
                Other->SetPosition(glm::vec2(SCPlayerPack.x, SCPlayerPack.y));
                Other->SetLookDir(SCPlayerPack.LookDir);
                Other->SetGunRotation(SCPlayerPack.GunRotation);
                Other->SetRecoilPosition(SCPlayerPack.RecoilPosition);
                Other->SetHP(SCPlayerPack.HP);
            }
            LeaveCriticalSection(&ThreadSection);
        }
    }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

    closesocket(ClientSocket);
    WSACleanup();
    DeleteCriticalSection(&ThreadSection);

    std::cout << "Thread end\n";
    return 0;
}