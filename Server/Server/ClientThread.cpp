#include "TCP_Header.h"
#include "ClientThread.h"
#include "Packet.h"
#include "Container.h"

// Ŭ���̾�Ʈ ������
DWORD WINAPI ClientThread(LPVOID lpParam) {
    bool ConnectState = true; // ���� ����, false�� �� �� ������ ��� ����
    int ReturnValue{};

    // ���� ��Ŷ Ÿ��
    uint8_t RecvPackType{};

    /////////////// Ŭ���̾�Ʈ ���� �� �ּ�
    SOCKET ClientSocket{}; // Ŭ���̾�Ʈ ����
    struct sockaddr_in ClientAddr {};  // Ŭ���̾�Ʈ ip �ּ�

    // ������ Ŭ�� ���� �ޱ�
    ClientInfo* ThisClient = (ClientInfo*)lpParam;
    ClientSocket = ThisClient->ClientSocket;
    ClientAddr = ThisClient->ClientAddr;
    inet_ntop(AF_INET, &ClientAddr.sin_addr, ThisClient->Addr, sizeof(ThisClient->Addr));



    while (ConnectState) {
        // Ŭ���̾�Ʈ�κ��� ��Ŷ Ÿ���� ���� �޴´�
        ReturnValue = recv(ClientSocket, (char*)&RecvPackType, sizeof(uint8_t), 0);
        std::println("RECV {}:", RecvPackType);
        if (ReturnValue == SOCKET_ERROR)
            break;




        // �÷��̾� ����
        if (RecvPackType == PACKET_TYPE_ENTER) {
            CS_INFO_PACKET CSInfoPack{};
            ReturnValue = recv(ClientSocket, (char*)&CSInfoPack, sizeof(CS_INFO_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR) 
                ConnectState = false;

            InputPacketInfo InputPackData{};
            strcpy(InputPackData.SCInfoPack.PlayerTag, CSInfoPack.PlayerTag);
            strcpy(InputPackData.SCInfoPack.GunType, CSInfoPack.GunType);
            InputPackData.PacketType = RecvPackType;
            InputPackData.Client = ThisClient;

            ClientPacketQueue.push(InputPackData);
        }

        // �÷��̾� ������
        else if (RecvPackType == PACKET_TYPE_MOVE) {
            CS_PLAYER_MOVE_PACKET CSMovePack{};

            ReturnValue = recv(ClientSocket, (char*)&CSMovePack, sizeof(CS_PLAYER_MOVE_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR) 
                ConnectState = false;

            InputPacketInfo InputPackData{};
            strcpy(InputPackData.SCMovePack.PlayerTag, CSMovePack.PlayerTag);
            InputPackData.SCMovePack.x = CSMovePack.x;
            InputPackData.SCMovePack.y = CSMovePack.y;
            InputPackData.SCMovePack.gun_rotation = CSMovePack.gun_rotation;
            InputPackData.SCMovePack.look_dir = CSMovePack.look_dir;
            InputPackData.PacketType = RecvPackType;
            InputPackData.Client = ThisClient;

            ClientPacketQueue.push(InputPackData);
        }
    }




    // ���� ���� �� ������ Ŭ���̾�Ʈ ��Ͽ��� ���� �� ���� �ݱ�
    EnterCriticalSection(&ThreadSection);
    auto It = std::find(ConnectedClients.begin(), ConnectedClients.end(), ThisClient);
    if (It != ConnectedClients.end())
        ConnectedClients.erase(It);
    if(NumConnected > 1) --NumConnected;
    LeaveCriticalSection(&ThreadSection);

    delete ThisClient;
    closesocket(ClientSocket);

    return 0;
}