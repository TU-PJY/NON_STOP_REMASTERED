#include "ClientThread.h"
#include "Packet.h"
#include "Scene.h"
#include "EngineHeader.h"

#include "OtherPlayer.h"
#include "Regular.h"

#include "GameOverMode.h"

#include <chrono>

// 클라이언트 스레드
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

    int flag = 1; // 비활성화하려면 1
    if (setsockopt(ClientSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag)) == SOCKET_ERROR) {
        std::cerr << "setsockopt(TCP_NODELAY) failed\n";
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

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
        err_quit("send() SendPacketType");

    // 클라이언트 자신의 상태를 패킷에 복사
    CS_INFO_PACKET CSInfoPack{};
    EnterCriticalSection(&ThreadSection);
    strcpy(CSInfoPack.PlayerTag, PlayerTag.c_str());
    strcpy(CSInfoPack.GunType, PlayerGunType.c_str());
    LeaveCriticalSection(&ThreadSection);

    //  패킷 보내기
    ReturnValue = send(ClientSocket, (char*)&CSInfoPack, sizeof(CS_INFO_PACKET), 0);
    if (ReturnValue == SOCKET_ERROR)
        err_quit("send() CS_INFO_PACKET");

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

    auto now = std::chrono::steady_clock::now();
    auto lastSendTime = now;

    while (ConnectState) {
        EnterCriticalSection(&ThreadSection);
        bool LocalConnectState = ConnectState;
        LeaveCriticalSection(&ThreadSection);

        // 연결 상태 해제 시 서버 스레드 종료
        if (!LocalConnectState)
            break;

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
            err_quit("send() CS_PLAYER_PACKET");

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
        // 몬스터 대미지 동기화
        EnterCriticalSection(&ThreadSection);
        auto LocalDamageList = DamageMonsterList;
        LeaveCriticalSection(&ThreadSection);

        if (!LocalDamageList.empty()) {
            int SendPacketType = PACKET_TYPE_MONSTER_DAMAGE;

            for (auto const& L : LocalDamageList) {
                ReturnValue = send(ClientSocket, (char*)&SendPacketType, sizeof(uint8_t), 0);
                if (ReturnValue == SOCKET_ERROR)
                    err_quit("recv() SendPacketType");

                CS_MONSTER_DAMAGE_PACKET CSMonsterDamagePack{};
                CSMonsterDamagePack.ID = L.ID;
                CSMonsterDamagePack.Damage = L.Damage;

                ReturnValue = send(ClientSocket, (char*)&CSMonsterDamagePack, sizeof(CS_MONSTER_DAMAGE_PACKET), 0);
                if (ReturnValue == SOCKET_ERROR)
                    err_quit("send() CS_MONSTER_DELETE_PACKET");
            }

            EnterCriticalSection(&ThreadSection);
            DamageMonsterList.clear();
            LeaveCriticalSection(&ThreadSection);
        }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
        // 몬스터 삭제 동기화
        EnterCriticalSection(&ThreadSection);
        auto LocalDeadList = DeleteMonsterList;
        LeaveCriticalSection(&ThreadSection);

        if (!LocalDeadList.empty()) {
            int SendPacketType = PACKET_TYPE_MONSTER_DELETE;

            for (auto& L : LocalDeadList) {
                ReturnValue = send(ClientSocket, (char*)&SendPacketType, sizeof(uint8_t), 0);
                if (ReturnValue == SOCKET_ERROR)
                    err_quit("recv() SendPacketType");

                CS_MONSTER_DELETE_PACKET CSMonsterDeletePack{};
                CSMonsterDeletePack.ID = L;

                ReturnValue = send(ClientSocket, (char*)&CSMonsterDeletePack, sizeof(CS_MONSTER_DELETE_PACKET), 0);
                if (ReturnValue == SOCKET_ERROR)
                    err_quit("send() CS_MONSTER_DELETE_PACKET");
            }

            EnterCriticalSection(&ThreadSection);
            DeleteMonsterList.clear();
            LeaveCriticalSection(&ThreadSection);
        }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
        // 플레이어 사망 동기화
        EnterCriticalSection(&ThreadSection);
        auto LocalDeletePlayerList = DeletePlayerList;
        LeaveCriticalSection(&ThreadSection);

        if (!LocalDeletePlayerList.empty()) {
            int SendPacketType = PACKET_TYPE_PLAYER_DELETE;

            for (const auto& L : LocalDeletePlayerList) {
                ReturnValue = send(ClientSocket, (char*)&SendPacketType, sizeof(uint8_t), 0);
                if (ReturnValue == SOCKET_ERROR)
                    err_quit("recv() SendPacketType");

                CS_PLAYER_DELETE_PACKET CSPlayerDeletePack{};
                strcpy(CSPlayerDeletePack.DeadPlayerTag, L.c_str());

                ReturnValue = send(ClientSocket, (char*)&CSPlayerDeletePack, sizeof(CS_PLAYER_DELETE_PACKET), 0);
                if (ReturnValue == SOCKET_ERROR)
                    err_quit("send() CS_MONSTER_DELETE_PACKET");
            }

            EnterCriticalSection(&ThreadSection);
            DeletePlayerList.clear();
            LeaveCriticalSection(&ThreadSection);
        }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

        // 패킷 타입 받기
        ReturnValue = recv(ClientSocket, (char*)&RecvPackType, sizeof(uint8_t), 0);
        if (ReturnValue == SOCKET_ERROR)
            err_quit("recv() PakcetType");

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
        // 게임 오버
        if (RecvPackType == PACKET_TYPE_GAME_OVER) {
            EnterCriticalSection(&ThreadSection);
            scene.SwitchMode(GameOverMode::Start);
            LeaveCriticalSection(&ThreadSection);
            break;
        }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
        // 플레이어 입장
        if (RecvPackType == PACKET_TYPE_ENTER) {
            std::cout << "RECV ENTER" << std::endl;

            SC_INFO_PACKET SCInfoPack{};
            ReturnValue = recv(ClientSocket, (char*)&SCInfoPack, sizeof(SC_INFO_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("recv() SC_INFO_PACKET");

            bool Duplicated{};

            EnterCriticalSection(&ThreadSection);
            // 중복되는 닉네임이 없을 경우 새로운 플레이어 추가
            auto It = std::find(begin(ConnectedPlayer), end(ConnectedPlayer), std::string(SCInfoPack.PlayerTag));
            if (It == ConnectedPlayer.end()) {
                // 새로운 닉네임 리스트 추가
                ConnectedPlayer.push_back(std::string(SCInfoPack.PlayerTag));

                // 타 플레이어 객체 추가 후 이름과 총 타입 지정
                scene.AddObject(new OtherPlayer, SCInfoPack.PlayerTag, LAYER_3);
                if (auto Other = scene.Find(SCInfoPack.PlayerTag); Other) {
                    Other->SetPlayerTag(SCInfoPack.PlayerTag);
                    Other->SetGunType(SCInfoPack.GunType);
                }

                Duplicated = false;
            }
            else
                Duplicated = true;
            LeaveCriticalSection(&ThreadSection);

            if (!Duplicated) {
                // 타 클라이언트가 서버에 접속할 경우 타 클라이언트에게 자신이 있음을 알림
                int SendPacketType = PACKET_TYPE_ENTER;
                ReturnValue = send(ClientSocket, (char*)&SendPacketType, sizeof(uint8_t), 0);
                if (ReturnValue == SOCKET_ERROR)
                    err_quit("send() SendPacketType");

                CS_INFO_PACKET CSInfoPack{};
                EnterCriticalSection(&ThreadSection);
                strcpy(CSInfoPack.PlayerTag, PlayerTag.c_str());
                strcpy(CSInfoPack.GunType, PlayerGunType.c_str());
                LeaveCriticalSection(&ThreadSection);

                ReturnValue = send(ClientSocket, (char*)&CSInfoPack, sizeof(CS_INFO_PACKET), 0);
                if (ReturnValue == SOCKET_ERROR)
                    err_quit("send() CS_INFO_PACKET");
            }
        }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
        // 플레이어 움직임
        if (RecvPackType == PACKET_TYPE_PLAYER) {
            SC_PLAYER_PACKET SCPlayerPack{};
            ReturnValue = recv(ClientSocket, (char*)&SCPlayerPack, sizeof(SC_PLAYER_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("recv() SC_PLAYER_PACKET");

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
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
        // 플레이어 사망 동기화
        if (RecvPackType == PACKET_TYPE_PLAYER_DELETE) {
            SC_PLAYER_DELETE_PACKET SCPlayerDeletePack{};
            ReturnValue = recv(ClientSocket, (char*)&SCPlayerDeletePack, sizeof(SC_PLAYER_DELETE_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("recv() SC_PLAYER_PACKET");

            std::string DeadTag = (std::string)SCPlayerDeletePack.DeadPlayerTag;

            // 사망처리된 플레이어의 닉네임을 가진 객체를 찾아 삭제한다.
            EnterCriticalSection(&ThreadSection);
            if (auto It = scene.Find(DeadTag); It) {
                scene.DeleteObject(It);

                auto Find = std::find(begin(ConnectedPlayer), end(ConnectedPlayer), DeadTag);
                if (Find != end(ConnectedPlayer))
                    ConnectedPlayer.erase(Find);
            }
            LeaveCriticalSection(&ThreadSection);
        }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

        // 몬스터 추가
        if (RecvPackType == PACKET_TYPE_MONSTER_ADD) {
            SC_MONSTER_ADD_PACKET SCMonsterAddPack{};
            ReturnValue = recv(ClientSocket, (char*)&SCMonsterAddPack, sizeof(SC_MONSTER_ADD_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("recv() SC_MONSTER_ADD_PACKET");

            // 몬스터 추가 패킷을 받으면 클라이언트에서 몬스터를 추가한다
            EnterCriticalSection(&ThreadSection);
            scene.AddObject(new RegularMonster(SCMonsterAddPack.AddDir, SCMonsterAddPack.ID, (std::string)SCMonsterAddPack.TrackTag), "monster", LAYER_2);
            LeaveCriticalSection(&ThreadSection);
        }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
        // 몬스터 대미지
        if (RecvPackType == PACKET_TYPE_MONSTER_DAMAGE) {
            SC_MONSTER_DAMAGE_PACKET SCMonsterDamagePack{};
            ReturnValue = recv(ClientSocket, (char*)&SCMonsterDamagePack, sizeof(SC_MONSTER_DAMAGE_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("recv() SC_MONSTER_DAMAGE_PACKET");

            // 해당하는 아이디를 가지는 몬스터가 있으면 대미지를 가한다.
            EnterCriticalSection(&ThreadSection);
            auto Range = scene.EqualRange("monster");
            for (auto It = Range.First; It != Range.End; It++) {
                if (It->second->GetID() == SCMonsterDamagePack.ID)
                    It->second->GiveDamage(SCMonsterDamagePack.Damage);
            }
            LeaveCriticalSection(&ThreadSection);
        }
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

        // 몬스터 삭제
        if (RecvPackType == PACKET_TYPE_MONSTER_DELETE) {
            SC_MONSTER_DELETE_PACKET SCMonsterDeletePack{};
            ReturnValue = recv(ClientSocket, (char*)&SCMonsterDeletePack, sizeof(SC_MONSTER_DELETE_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("recv() SC_MONSTER_DELETE_PACKET");

            std::cout << "Get ID: " << SCMonsterDeletePack.ID << std::endl;

            // 해당하는 아이디를 가지는 몬스터가 있으면 삭제한다
            EnterCriticalSection(&ThreadSection);
            auto Range = scene.EqualRange("monster");
            for (auto It = Range.First; It != Range.End; It++) {
                if (It->second->GetID() == SCMonsterDeletePack.ID)
                    scene.DeleteObject(It->second);
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