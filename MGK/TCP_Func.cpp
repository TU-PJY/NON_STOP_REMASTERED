#include "TCP_Header.h"

// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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

// 소켓 함수 오류 출력
void err_display(int errcode) {
    LPVOID lpMsgBuf;
    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, errcode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (char*)&lpMsgBuf, 0, NULL);
    printf("[오류] %s\n", (char*)lpMsgBuf);
    LocalFree(lpMsgBuf);
}

char SERVER_IP[20];  // 클라이언트 주소
CRITICAL_SECTION ThreadSection;

// 연결된 플레이어들의 닉네임
std::vector<std::string> ConnectedPlayer;

bool ConnectState;
std::string PlayerGunType;
std::string PlayerTag;
bool PlayerReadyState;

std::vector<int> DeleteMonsterList;

std::vector<DamageInfo> DamageMonsterList;