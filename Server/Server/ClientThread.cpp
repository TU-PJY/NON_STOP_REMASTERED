#include "TCP_Header.h"
#include "ClientThread.h"
#include "Packet.h"
#include "Container.h"
// 클라이언트 스레드
DWORD WINAPI ClientThread(LPVOID lpParam) {
    bool ConnectState = true; // 연결 상태, false가 될 시 스레드 즉시 종료
    int ReturnValue{};

    // 받은 패킷 타입
    uint8_t RecvPackType{};

    /////////////// 클라이언트 소켓 및 주소
    SOCKET ClientSocket{}; // 클라이언트 소켓
    struct sockaddr_in ClientAddr {};  // 클라이언트 ip 주소

    // 접속한 클라 정보 받기
    ClientInfo* ThisClient = (ClientInfo*)lpParam;
    ClientSocket = ThisClient->ClientSocket;
    ClientAddr = ThisClient->ClientAddr;
    inet_ntop(AF_INET, &ClientAddr.sin_addr, ThisClient->Addr, sizeof(ThisClient->Addr));

    while (ConnectState) {
        // 클라이언트로부터 패킷 타입을 먼저 받는다
        ReturnValue = recv(ClientSocket, (char*)&RecvPackType, sizeof(uint8_t), 0);
        std::println("RECV {}:", RecvPackType);
        if (ReturnValue == SOCKET_ERROR)
            break;

        // 플레이어 입장
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

        // 플레이어 움직임
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

        // 타 클라이언트에서 몬스터 삭제 이벤트가 발생한 경우 나머지 클라에서도 해당 몬스터를 삭제하도록 한다
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




    // 접속 종료 시 접속한 클라이언트 목록에서 제거 후 소켓 닫기
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