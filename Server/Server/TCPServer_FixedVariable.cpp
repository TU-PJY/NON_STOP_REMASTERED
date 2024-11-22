#include "TCP_Header.h" // 에러 메시지 함수 및 TCP 서버에 필요한 헤더 포함
#include "Packet.h"
//#include "LockFreeQueue.h"
#include <queue>
#include <print>

// 클라이언트 정보
struct ClientInfo {
    SOCKET ClientSocket;
    struct sockaddr_in ClientAddr;
    char Addr[INET_ADDRSTRLEN]{};  // 클라이언트 ip 주소 문자열
};

// 큐에서 처리하는 서버 -> 클라이언트 구조체
typedef struct {
    uint8_t PacketType;  // 패킷 타입
    void* PacketPtr;  // 클라이언트로 전송할 패킷을 가리키는 포인터
    ClientInfo* Client; // 클라이언트 정보
} ClientPacketInfo;

std::vector<ClientInfo*> ConnectedClients; // 연결된 클라이언트 목록
std::queue<ClientPacketInfo> ClientPacketQueue; // 클라이언트 패킷 처리 큐, 락 프리 큐이므로 임계구역 사용 안해도 됨
CRITICAL_SECTION ThreadSection; // 임계구역

////////////////////////////////////////////////////////////////

// 클라이언트 스레드
DWORD WINAPI ClientThread(LPVOID lpParam) {
    bool ConnectState = true; // 연결 상태, false가 될 시 스레드 즉시 종료
    int ReturnValue{};

    // 받은 패킷 타입
    uint8_t RecievePacketType{};

    // 다른 클라이언트로 보낼 패킷 타입
    uint8_t SendPacketType{};

    /////////////// 클라이언트 소켓 및 주소
    SOCKET ClientSocket{}; // 클라이언트 소켓
    struct sockaddr_in ClientAddr {};  // 클라이언트 ip 주소

    // 접속한 클라 정보 받기
    ClientInfo* ThisClient = (ClientInfo*)lpParam;
    ClientSocket = ThisClient->ClientSocket;
    ClientAddr = ThisClient->ClientAddr;
    inet_ntop(AF_INET, &ClientAddr.sin_addr, ThisClient->Addr, sizeof(ThisClient->Addr));

    // 클라이언트 패킷 처리 구조체
    ClientPacketInfo PacketInfo{};

    while (ConnectState) {
        // 클라이언트로부터 패킷 타입을 먼저 받는다
        ReturnValue = recv(ClientSocket, (char*)&RecievePacketType, sizeof(uint8_t), 0);
        if (ReturnValue == SOCKET_ERROR)
            ConnectState = false;

        // 받은 패킷 타입에 따라 다른 패킷 타입을 받는다
        switch (RecievePacketType) {
        case PACKET_TYPE_LOBBY:
            SC_LOBBY_PACKET* SC_LobbyPacket = new SC_LOBBY_PACKET;
            CS_LOBBY_PACKET CS_LobbyPacket{};

            // CS_LobbyPacket 받기
            ReturnValue = recv(ClientSocket, (char*)&CS_LobbyPacket, sizeof(CS_LOBBY_PACKET), 0);
            if (ReturnValue == SOCKET_ERROR)
                ConnectState = false;

            // 플레이어 태그, 플레이어 총 타입, 플레이어 준비상태 데이터 복사
            strncpy((char*)SC_LobbyPacket->PlayerTag, (const char*)CS_LobbyPacket.PlayerTag, sizeof(CS_LobbyPacket.PlayerTag));
            strncpy((char*)SC_LobbyPacket->GunType, (const char*)CS_LobbyPacket.GunType, sizeof(CS_LobbyPacket.GunType));
            SC_LobbyPacket->ReadyState = CS_LobbyPacket.ReadyState;

            // 큐에 클라이언트 클라이언트로 보낼 패킷 정보 추가
            EnterCriticalSection(&ThreadSection);
            ClientPacketQueue.push({ RecievePacketType, SC_LobbyPacket, ThisClient });
            LeaveCriticalSection(&ThreadSection);
            break;
        }
    }

    // 접속 종료 시 접속한 클라이언트 목록에서 제거 후 소켓 닫기
    EnterCriticalSection(&ThreadSection);
    auto It = std::find(ConnectedClients.begin(), ConnectedClients.end(), ThisClient);
    if (It != ConnectedClients.end())
        ConnectedClients.erase(It);
    LeaveCriticalSection(&ThreadSection);

    delete ThisClient;
    closesocket(ClientSocket);

    return 0;
}

// 서버 -> 클라이언트 큐 스레드
DWORD WINAPI ClientQueueThread(LPVOID lpParam) {
    ClientPacketInfo PacketInfo{};
    int ReturnValue{};

    while (true) {
        EnterCriticalSection(&ThreadSection);
        // 큐가 비어있지 않으면 큐에 있는 원소를 하나씩 꺼낸다.
        while (!ClientPacketQueue.empty()) {
            PacketInfo = ClientPacketQueue.front();
            ClientPacketQueue.pop();

            // 패킷타입에 따라 다른 패킷을 클라이언트에 전송한다.
            // 자신의 클라이언트를 제외한 나머지 클라이언트에 전송한다.
            switch (PacketInfo.PacketType) {
            case PACKET_TYPE_LOBBY:
                for(auto const& Other : ConnectedClients) {
                    if (Other != PacketInfo.Client) {
                        ReturnValue = send(Other->ClientSocket, (char*)&PacketInfo.PacketType, sizeof(uint8_t), 0);
                        if (ReturnValue == SOCKET_ERROR)
                            continue;

                        ReturnValue = send(Other->ClientSocket, (char*)static_cast<SC_LOBBY_PACKET*>(PacketInfo.PacketPtr), sizeof(SC_LOBBY_PACKET), 0);
                        if (ReturnValue == SOCKET_ERROR) 
                            continue;
                    }
                }

                // 가리키는 패킷 구조체 객체 삭제
                delete PacketInfo.PacketPtr;
                break;
            }
        }
        LeaveCriticalSection(&ThreadSection);
    }

    return 0;
}

int main(int argc, char* argv[]) {
    int ReturnValue{};

    SOCKET ClientSocket{}; // 클라이언트 소켓
    struct sockaddr_in ClientAddr {};  // 클라이언트 주소
    int AddrLength{};  // 주소 길이

   // 윈속 초기화
    WSADATA WSA;
    if (WSAStartup(MAKEWORD(2, 2), &WSA) != 0)
        return 1;

    // 소켓 생성
    SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ListenSocket == INVALID_SOCKET) 
        err_quit("socket()");

    // bind()
    struct sockaddr_in ServerAddr;
    memset(&ServerAddr, 0, sizeof(ServerAddr));
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    ServerAddr.sin_port = htons(SERVER_PORT);
    ReturnValue = bind(ListenSocket, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
    if (ReturnValue == SOCKET_ERROR) 
        err_quit("bind()");

    // listen()
    ReturnValue = listen(ListenSocket, SOMAXCONN);
    if (ReturnValue == SOCKET_ERROR) 
        err_quit("listen()");

    // 임계구역 초기화
    InitializeCriticalSection(&ThreadSection);

    // 큐 처리 스레드 생성
    HANDLE Thread = CreateThread(NULL, 0, ClientQueueThread, NULL, 0, NULL);
    if (!Thread)
        std::cout << "Failed to create thread." << std::endl;
    else
        CloseHandle(Thread);

    while (true) {
        if (ConnectedClients.size() < MAX_CLIENT) {
            // accept()
            AddrLength = sizeof(ClientAddr);
            ClientSocket = accept(ListenSocket, (struct sockaddr*)&ClientAddr, &AddrLength);
            if (ClientSocket == INVALID_SOCKET) {
                err_display("accept()");
                break;
            }

            // 클라이언트가 접속하면 클라이언트의 소켓과 주소를 스레드 파라미터로 전달
            ClientInfo* Client = new ClientInfo{ ClientSocket, ClientAddr, NULL };

            // 연결된 클라이언트 목록에 추가
            ConnectedClients.push_back(Client);

            // 새로운 스레드를 생성하여 작업 시작
            HANDLE Thread = CreateThread(NULL, 0, ClientThread, (LPVOID)Client, 0, NULL);
            if (!Thread) {
                std::cout << "Failed to create thread." << std::endl;
                delete Client;
                closesocket(ClientSocket);
            }
            else
                CloseHandle(Thread);
        }
    }

    DeleteCriticalSection(&ThreadSection);

    // 소켓 닫기
    closesocket(ListenSocket);
    WSACleanup();
}