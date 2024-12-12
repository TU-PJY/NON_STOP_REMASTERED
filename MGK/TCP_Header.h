#pragma once
#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기
#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <set>
#include <print>
#include <map>
#include "Config.h"

#define SERVER_PORT 9000

// 클라이언트 정보
typedef struct {
    SOCKET ClientSocket;
    struct sockaddr_in ClientAddr;
    char Addr[INET_ADDRSTRLEN];  // 클라이언트 ip 주소 문자열
} ClientInfo;

// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg);

// 소켓 함수 오류 출력
void err_display(const char* msg);

// 소켓 함수 오류 출력
void err_display(int errcode);

extern char SERVER_IP[20]; // 클라이언트 주소
extern CRITICAL_SECTION ThreadSection;

extern std::vector<std::string> ConnectedPlayer;
extern int NumPlayerConnected;

// 플레이어 전역 변수 여기에 선언
extern bool ConnectState;
extern std::string PlayerGunType;
extern std::string PlayerTag;
extern bool PlayerReadyState;

// 대미지 입은 몬스터 ID및 대미지
typedef struct {
    int ID;
    int Damage;
}DamageInfo;

// 죽은 몬스터 리스트
extern std::vector<int> DeleteMonsterList;

// 대미지 입은 몬스터 리스트
extern std::vector<DamageInfo> DamageMonsterList;