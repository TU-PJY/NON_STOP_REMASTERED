#include "TCP_Header.h"
#include "ClientThread.h"
#include "Packet.h"
#include "Container.h"

// Ŭ���̾�Ʈ ������
DWORD WINAPI ClientThread(LPVOID lpParam) {
    bool ConnectState = true; // ���� ����, false�� �� �� ������ ��� ����
    int ReturnValue{};

    // ���� ��Ŷ Ÿ��
    uint8_t RecievePacketType{};

    /////////////// Ŭ���̾�Ʈ ���� �� �ּ�
    SOCKET ClientSocket{}; // Ŭ���̾�Ʈ ����
    struct sockaddr_in ClientAddr {};  // Ŭ���̾�Ʈ ip �ּ�

    // ������ Ŭ�� ���� �ޱ�
    ClientInfo* ThisClient = (ClientInfo*)lpParam;
    ClientSocket = ThisClient->ClientSocket;
    ClientAddr = ThisClient->ClientAddr;
    inet_ntop(AF_INET, &ClientAddr.sin_addr, ThisClient->Addr, sizeof(ThisClient->Addr));

    ClientPacketInfo C_PacketInfo{};
    CS_LOBBY_PACKET CS_LobbyPacket{};

    while (ConnectState) {
        // Ŭ���̾�Ʈ�κ��� ��Ŷ Ÿ���� ���� �޴´�
        ReturnValue = recv(ClientSocket, (char*)&RecievePacketType, sizeof(uint8_t), 0);
        std::println("RECV {}:", RecievePacketType);
        if (ReturnValue == SOCKET_ERROR)
            break;

        // ���� ��Ŷ Ÿ�Կ� ���� �ٸ� ��Ŷ Ÿ���� �޴´�
        switch (RecievePacketType) {
        case PACKET_TYPE_ENTER:
            // CS_LobbyPacket �ޱ�
            memset(&CS_LobbyPacket, 0, sizeof(CS_LOBBY_PACKET));
            ReturnValue = recv(ClientSocket, (char*)&CS_LobbyPacket, sizeof(CS_LOBBY_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR) {
                ConnectState = false;
                break;
            }

            // ���� ���� �� ����ü �ʱ�ȭ
            // �÷��̾� �±�, �÷��̾� �� Ÿ��, �÷��̾� �غ���� ������ ����
            memset(&C_PacketInfo, 0, sizeof(ClientPacketInfo));
            strcpy(C_PacketInfo.SC_LobbyPacket.PlayerTag, CS_LobbyPacket.PlayerTag);
            strcpy(C_PacketInfo.SC_LobbyPacket.GunType, CS_LobbyPacket.GunType);
            C_PacketInfo.SC_LobbyPacket.ReadyState = CS_LobbyPacket.ReadyState;
            C_PacketInfo.PacketType = RecievePacketType;
            C_PacketInfo.Client = ThisClient;

            // ť�� Ŭ���̾�Ʈ Ŭ���̾�Ʈ�� ���� ��Ŷ ���� �߰�
            ClientPacketQueue.push(C_PacketInfo);
            break;

        case PACKET_TYPE_MOVE:
        {
            CS_PLAYER_MOVE_PACKET CS_MovePacket{};
            ClientPacketInfo C_PacketInfo{};

            ReturnValue = recv(ClientSocket, (char*)&CS_MovePacket, sizeof(CS_PLAYER_MOVE_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR) {
                ConnectState = false;
                break;
            }

            strcpy(C_PacketInfo.SC_MovePacket.PlayerTag, CS_MovePacket.PlayerTag);
            C_PacketInfo.SC_MovePacket.x = CS_MovePacket.x;
            C_PacketInfo.SC_MovePacket.y = CS_MovePacket.y;
            C_PacketInfo.PacketType = RecievePacketType;
            C_PacketInfo.Client = ThisClient;

            std::println("Tag: {}, X: {}, Y : {}", CS_MovePacket.PlayerTag, CS_MovePacket.x, CS_MovePacket.y);

            ClientPacketQueue.push(C_PacketInfo);
        }
        break;
        }
    }

    // ���� ���� �� ������ Ŭ���̾�Ʈ ��Ͽ��� ���� �� ���� �ݱ�
    EnterCriticalSection(&ThreadSection);
    auto It = std::find(ConnectedClients.begin(), ConnectedClients.end(), ThisClient);
    if (It != ConnectedClients.end())
        ConnectedClients.erase(It);
    LeaveCriticalSection(&ThreadSection);

    delete ThisClient;
    closesocket(ClientSocket);

    return 0;
}