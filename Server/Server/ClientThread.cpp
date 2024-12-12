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
        if (RecvPackType == PACKET_TYPE_PLAYER) {
            CS_PLAYER_PACKET CSPlayerPack{};

            ReturnValue = recv(ClientSocket, (char*)&CSPlayerPack, sizeof(CS_PLAYER_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR) 
                ConnectState = false;

            InputPacketInfo InputPackData{};
            strcpy(InputPackData.SCPlayerPack.PlayerTag, CSPlayerPack.PlayerTag);
            InputPackData.SCPlayerPack.x = CSPlayerPack.x;
            InputPackData.SCPlayerPack.y = CSPlayerPack.y;
            InputPackData.SCPlayerPack.LookDir = CSPlayerPack.LookDir;
            InputPackData.SCPlayerPack.GunRotation = CSPlayerPack.GunRotation;
            InputPackData.SCPlayerPack.RecoilPosition = CSPlayerPack.RecoilPosition;
            InputPackData.SCPlayerPack.HP = CSPlayerPack.HP;
            InputPackData.PacketType = RecvPackType;
            InputPackData.Client = ThisClient;

            ClientPacketQueue.push(InputPackData);
        }

        // Ÿ Ŭ���̾�Ʈ���� ���� ���� �̺�Ʈ�� �߻��� ��� ������ Ŭ�󿡼��� �ش� ���͸� �����ϵ��� �Ѵ�
        if (RecvPackType == PACKET_TYPE_MONSTER_DELETE) {
            CS_MONSTER_DELETE_PACKET CSMonsterDeletePack{};

            ReturnValue = recv(ClientSocket, (char*)&CSMonsterDeletePack, sizeof(CS_MONSTER_DELETE_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                ConnectState = false;

            InputPacketInfo InputPackData{};
            InputPackData.SCMonsterDeletePack.ID = CSMonsterDeletePack.ID;
            InputPackData.Client = ThisClient;
            InputPackData.PacketType = RecvPackType;

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