#include "TCP_Header.h"

// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg) {
    LPVOID lpMsgBuf;
    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (char*)&lpMsgBuf, 0, NULL);
    MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);
    LocalFree(lpMsgBuf);
    exit(1);
}

// ���� �Լ� ���� ���
void err_display(const char* msg) {
    LPVOID lpMsgBuf;
    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (char*)&lpMsgBuf, 0, NULL);
    printf("[%s] %s\n", msg, (char*)lpMsgBuf);
    LocalFree(lpMsgBuf);
}

// ���� �Լ� ���� ���
void err_display(int errcode) {
    LPVOID lpMsgBuf;
    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, errcode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (char*)&lpMsgBuf, 0, NULL);
    printf("[����] %s\n", (char*)lpMsgBuf);
    LocalFree(lpMsgBuf);
}

char SERVER_IP[20];  // Ŭ���̾�Ʈ �ּ�
CRITICAL_SECTION ThreadSection;

// ����� �÷��̾���� �г���
std::vector<std::string> ConnectedPlayer;

bool ConnectState;
std::string PlayerGunType;
std::string PlayerTag;
bool PlayerReadyState;

std::vector<int> DeleteMonsterList;

std::vector<DamageInfo> DamageMonsterList;