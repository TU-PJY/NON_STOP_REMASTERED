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

    // ���� �ʱ�ȭ
    WSADATA WSA;
    if (WSAStartup(MAKEWORD(2, 2), &WSA) != 0)
        return 1;

    // Ŭ���̾�Ʈ ���� ����
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

    // ���� ���� Ȱ��ȭ
    EnterCriticalSection(&ThreadSection);
    ConnectState = true;
    LeaveCriticalSection(&ThreadSection);

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

    SC_INFO_PACKET SCInfoPack{};

    // ��Ŷ Ÿ���� ������.
    int SendPacketType = PACKET_TYPE_ENTER;
    ReturnValue = send(ClientSocket, (char*)&SendPacketType, sizeof(uint8_t), 0);
    std::cout << "Send Packet:" << SendPacketType << "\n";
    if (ReturnValue == SOCKET_ERROR)
        err_quit("send() PACKET_TYPE_LOBBY");

    // Ŭ���̾�Ʈ �ڽ��� ���¸� ��Ŷ�� ����
    CS_INFO_PACKET CSInfoPack{};
    EnterCriticalSection(&ThreadSection);
    strcpy(CSInfoPack.PlayerTag, PlayerTag.c_str());
    strcpy(CSInfoPack.GunType, PlayerGunType.c_str());
    LeaveCriticalSection(&ThreadSection);

    //  ��Ŷ ������
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

        // ���� ���� ���� �� ���� ������ ����
        if (!LocalConnectState)
            break;

        // �÷��� ��� ��Ŷ Ÿ�� ����
        if (LocalModeName == "PlayMode") {
            // ������ ��Ŷ ����
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
            
            // �ڽ��� ������ Ÿ Ŭ���̾�Ʈ�� �����Ѵ�
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

        // ��Ŷ Ÿ�� �ޱ�
        ReturnValue = recv(ClientSocket, (char*)&RecvPackType, sizeof(uint8_t), 0);
        if (ReturnValue == SOCKET_ERROR)
            err_quit("recv() PakcetType");

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

        // �÷��̾� ����
        if (RecvPackType == PACKET_TYPE_ENTER) {
            SC_INFO_PACKET SCInfoPack{};
            ReturnValue = recv(ClientSocket, (char*)&SCInfoPack, sizeof(SC_INFO_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("recv() SC_LOBBY_PACKET");

            EnterCriticalSection(&ThreadSection);

            // �ߺ��Ǵ� �г����� ���� ��� ���ο� �÷��̾� �߰�
            auto It = std::find(begin(ConnectedPlayer), end(ConnectedPlayer), std::string(SCInfoPack.PlayerTag));

           if(It == ConnectedPlayer.end()) {
                // ���ο� �г��� ����Ʈ �߰�
                ConnectedPlayer.push_back(std::string(SCInfoPack.PlayerTag));

                // Ÿ �÷��̾� ��ü �߰� �� �̸��� �� Ÿ�� ����
                scene.AddObject(new OtherPlayer, SCInfoPack.PlayerTag, LAYER_3);
                if (auto Other = scene.Find(SCInfoPack.PlayerTag); Other) {
                    Other->SetPlayerTag(SCInfoPack.PlayerTag);
                    Other->SetGunType(SCInfoPack.GunType);
                }
            }

           LeaveCriticalSection(&ThreadSection);

            // Ÿ Ŭ���̾�Ʈ�� ������ ������ ��� Ÿ Ŭ���̾�Ʈ���� �ڽ��� ������ �˸�
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

        // �÷��̾� ������
        else if (PACKET_TYPE_PLAYER) {
            SC_PLAYER_PACKET SCPlayerPack{};
            ReturnValue = recv(ClientSocket, (char*)&SCPlayerPack, sizeof(SC_PLAYER_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("recv() SC_LOBBY_PACKET");

            CS_PLAYER_PACKET CSPlayerPack{};

            // Ÿ Ŭ���̾�Ʈ �÷��̾� ��ü ������Ʈ
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