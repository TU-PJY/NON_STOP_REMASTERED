#include "TCP_Header.h" // 에러 메시지 함수 및 TCP 서버에 필요한 헤더 포함
#include "ClientThread.h"
#include "QueueThread.h"
#include "MonsterManager.h"
#include "Container.h"

int NumConnected;
int NumDead;

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
        std::println("Failed to create queue thread.");
    else
        CloseHandle(Thread);

    // 몬스터 추가 스레드 생성
    HANDLE MThread = CreateThread(NULL, 0, MonsterThread, NULL, 0, NULL);
    if (!MThread)
        std::println("Failed to create monster thread.");
    else
        CloseHandle(MThread);

    int ID = 0;
    NumConnected = 0;

    while (true) {
        if (NumConnected < MAX_CLIENT) {
            // accept()
            AddrLength = sizeof(ClientAddr);
            ClientSocket = accept(ListenSocket, (struct sockaddr*)&ClientAddr, &AddrLength);
            if (ClientSocket == INVALID_SOCKET) {
                err_display("accept()");
                break;
            }

            // 접속 수 증가
            ++NumConnected;

            // 클라이언트가 접속하면 클라이언트의 소켓과 주소를 스레드 파라미터로 전달
            ClientInfo* Client = new ClientInfo;
            Client->ClientSocket = ClientSocket;
            Client->ClientAddr = ClientAddr;
            Client->ID = ID;

            // 연결된 클라이언트 목록에 추가
            ConnectedClients.push_back(Client);

            // 새로운 스레드를 생성하여 작업 시작
            HANDLE Thread = CreateThread(NULL, 0, ClientThread, (LPVOID)Client, 0, NULL);
            if (!Thread) {
                std::println("Failed to create client thread.");
                delete Client;
                closesocket(ClientSocket);
            }
            else {
                CloseHandle(Thread);
            }
        }
    }
    
    // 임계구역 삭제
    DeleteCriticalSection(&ThreadSection);

    // 소켓 닫기
    closesocket(ListenSocket);
    WSACleanup();
}