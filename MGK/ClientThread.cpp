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

    SC_LOBBY_PACKET SC_LobbyPacket{};
    CS_LOBBY_PACKET CS_LobbyPacket{};
    OTHER_CLIENT OtherClient{};

    while (ConnectState) {
        // 패킷 타입을 보낸다.
        int SendPacketType = PACKET_TYPE_LOBBY;
        ReturnValue = send(ClientSocket, (char*)&SendPacketType, sizeof(uint8_t), 0);
        if (ReturnValue == SOCKET_ERROR)
            err_quit("send() PACKET_TYPE_LOBBY");

        // 클라이언트 자신의 상태를 패킷에 복사
        memset(&CS_LobbyPacket, 0, sizeof(CS_LOBBY_PACKET));
        EnterCriticalSection(&ThreadSection);
        strcpy(CS_LobbyPacket.PlayerTag, PlayerTag.c_str());
        strcpy(CS_LobbyPacket.GunType, PlayerGunType.c_str());
        CS_LobbyPacket.ReadyState = PlayerReadyState;
        LeaveCriticalSection(&ThreadSection);

        //  패킷 보내기
        ReturnValue = send(ClientSocket, (char*)&CS_LobbyPacket, sizeof(CS_LOBBY_PACKET), 0);
        if (ReturnValue == SOCKET_ERROR)
            err_quit("send() CS_LOBBY_PACKET");

        // 패킷 타입 받기
        ReturnValue = recv(ClientSocket, (char*)&RecievePacketType, sizeof(uint8_t), 0);
        if (ReturnValue == SOCKET_ERROR)
            err_quit("recv() PakcetType");

        // 받은 패킷 처리
        switch (RecievePacketType) {
            // 타 클라이언트 정보 패킷
        case PACKET_TYPE_LOBBY:
        {
            memset(&SC_LobbyPacket, 0, sizeof(SC_LOBBY_PACKET));
            ReturnValue = recv(ClientSocket, (char*)&SC_LobbyPacket, sizeof(SC_LOBBY_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                err_quit("recv() SC_LOBBY_PACKET");

            bool clientFound = false;

            EnterCriticalSection(&ThreadSection);
            if (ConnectedPlayer.size() < 3) {
                for (auto& Other : ConnectedPlayer) {
                    if (Other.PlayerTag == std::string(SC_LobbyPacket.PlayerTag)) {
                        Other.PlayerTag = std::string(SC_LobbyPacket.PlayerTag);
                        Other.GunType = std::string(SC_LobbyPacket.GunType);
                        Other.ReadyState = SC_LobbyPacket.ReadyState;
                        clientFound = true;
                        break;
                    }
                }

                if (!clientFound) {
                    OTHER_CLIENT newClient;
                    newClient.PlayerTag = std::string(SC_LobbyPacket.PlayerTag);
                    newClient.GunType = std::string(SC_LobbyPacket.GunType);
                    newClient.ReadyState = SC_LobbyPacket.ReadyState;
                    ConnectedPlayer.push_back(newClient);
                }
            }
            LeaveCriticalSection(&ThreadSection);
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