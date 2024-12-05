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

    //SC_INFO_PACKET SC_LobbyPacket{};

    //// ��Ŷ Ÿ���� ������.
    //int SendPacketType = PACKET_TYPE_ENTER;
    //ReturnValue = send(ClientSocket, (char*)&SendPacketType, sizeof(uint8_t), 0);
    //std::cout << "Send Packet:" << SendPacketType << "\n";
    //if (ReturnValue == SOCKET_ERROR)
    //    err_quit("send() PACKET_TYPE_LOBBY");

    //// Ŭ���̾�Ʈ �ڽ��� ���¸� ��Ŷ�� ����
    //CS_LOBBY_PACKET CS_LobbyPacket{};
    //EnterCriticalSection(&ThreadSection);
    //strcpy(CS_LobbyPacket.PlayerTag, PlayerTag.c_str());
    //strcpy(CS_LobbyPacket.GunType, PlayerGunType.c_str());
    //LeaveCriticalSection(&ThreadSection);

    ////  ��Ŷ ������
    //ReturnValue = send(ClientSocket, (char*)&CS_LobbyPacket, sizeof(CS_LOBBY_PACKET), 0);
    //if (ReturnValue == SOCKET_ERROR)
    //    err_quit("send() CS_LOBBY_PACKET");

    while (ConnectState) {
        EnterCriticalSection(&ThreadSection);
        std::string LocalModeName = scene.Mode();
        LeaveCriticalSection(&ThreadSection);

        // �÷��� ��� ��Ŷ Ÿ�� ����
        if (LocalModeName == "PlayMode") {
            // ������ ��Ŷ ����
            int SendPacketType = PACKET_TYPE_MOVE;
            ReturnValue = send(ClientSocket, (char*)&SendPacketType, sizeof(uint8_t), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("recv() PakcetType");

            CS_PLAYER_MOVE_PACKET CSMovePack{};
            if (auto Player = scene.Find("player"); Player) {
                strcpy(CSMovePack.PlayerTag, PlayerTag.c_str());
                glm::vec2 Position = Player->GetPosition();
                CSMovePack.x = Position.x;
                CSMovePack.y = Position.y;
            }

            ReturnValue = send(ClientSocket, (char*)&CSMovePack, sizeof(CS_PLAYER_MOVE_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("send() CS_LOBBY_PACKET");
        }

        // ��Ŷ Ÿ�� �ޱ�
        ReturnValue = recv(ClientSocket, (char*)&RecvPackType, sizeof(uint8_t), 0);
        std::cout << "Recv Packet:" << RecvPackType << "\n";
        if (ReturnValue == SOCKET_ERROR)
            err_quit("recv() PakcetType");

        // �÷��̾� ����
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
                // ���ο� Ŭ���̾�Ʈ �߰�
                OTHER_CLIENT newClient{};
                newClient.PlayerTag = std::string(SCInfoPack.PlayerTag);
                newClient.GunType = std::string(SCInfoPack.GunType);
                ConnectedPlayer.push_back(newClient);

                // Ÿ �÷��̾� ��ü �߰� �� �̸��� �� Ÿ�� ����
                scene.AddObject(new OtherPlayer, newClient.PlayerTag.c_str(), LAYER_3);
                if (auto Other = scene.Find(newClient.PlayerTag.c_str()); Other) {
                    Other->SetPlayerTag(newClient.PlayerTag);
                    Other->SetGunType(newClient.GunType);
                }
            }

           LeaveCriticalSection(&ThreadSection);


            // �÷��̾ ������ ������ ��� Ÿ Ŭ���̾�Ʈ���� �ڽ��� ������ �˸�
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

        // �÷��̾� ������
        else if (PACKET_TYPE_MOVE) {
            SC_PLAYER_MOVE_PACKET SC_MovePacket{};
            ReturnValue = recv(ClientSocket, (char*)&SC_MovePacket, sizeof(SC_PLAYER_MOVE_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("recv() SC_LOBBY_PACKET");

            std::cout << "Tag: " << SC_MovePacket.PlayerTag << "\n X: " << SC_MovePacket.x << "\n y:" << SC_MovePacket.y << "\n";
        }
    }

    closesocket(ClientSocket);
    WSACleanup();
    DeleteCriticalSection(&ThreadSection);

    std::cout << "Thread end\n";
    return 0;
}