#include "TCP_Header.h" // ���� �޽��� �Լ� �� TCP ������ �ʿ��� ��� ����
#include "ClientThread.h"
#include "QueueThread.h"
#include "MonsterManager.h"
#include "Container.h"

int NumConnected;
int NumDead;

int main(int argc, char* argv[]) {
    int ReturnValue{};

    SOCKET ClientSocket{}; // Ŭ���̾�Ʈ ����
    struct sockaddr_in ClientAddr {};  // Ŭ���̾�Ʈ �ּ�
    int AddrLength{};  // �ּ� ����

    // ���� �ʱ�ȭ
    WSADATA WSA;
    if (WSAStartup(MAKEWORD(2, 2), &WSA) != 0)
        return 1;

    // ���� ����
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

    // �Ӱ豸�� �ʱ�ȭ
    InitializeCriticalSection(&ThreadSection);

    // ť ó�� ������ ����
    HANDLE Thread = CreateThread(NULL, 0, ClientQueueThread, NULL, 0, NULL);
    if (!Thread)
        std::println("Failed to create queue thread.");
    else
        CloseHandle(Thread);

    // ���� �߰� ������ ����
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

            // ���� �� ����
            ++NumConnected;

            // Ŭ���̾�Ʈ�� �����ϸ� Ŭ���̾�Ʈ�� ���ϰ� �ּҸ� ������ �Ķ���ͷ� ����
            ClientInfo* Client = new ClientInfo;
            Client->ClientSocket = ClientSocket;
            Client->ClientAddr = ClientAddr;
            Client->ID = ID;

            // ����� Ŭ���̾�Ʈ ��Ͽ� �߰�
            ConnectedClients.push_back(Client);

            // ���ο� �����带 �����Ͽ� �۾� ����
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
    
    // �Ӱ豸�� ����
    DeleteCriticalSection(&ThreadSection);

    // ���� �ݱ�
    closesocket(ListenSocket);
    WSACleanup();
}