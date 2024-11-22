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
#include <boost/lockfree/queue.hpp>
#include <vector>
#include <print>
#include "Config.h"

// 클라이언트 정보
typedef struct {
    SOCKET ClientSocket;
    struct sockaddr_in ClientAddr;
    char Addr[INET_ADDRSTRLEN];  // 클라이언트 ip 주소 문자열
} ClientInfo;

// 큐에서 처리하는 서버 -> 클라이언트 구조체
typedef struct {
    uint8_t PacketType;  // 패킷 타입
    void* PacketPtr;  // 클라이언트로 전송할 패킷을 가리키는 포인터
    ClientInfo* Client; // 클라이언트 정보
} ClientPacketInfo;

extern std::vector<ClientInfo*> ConnectedClients; // 연결된 클라이언트 목록
extern boost::lockfree::queue<ClientPacketInfo> ClientPacketQueue; // 클라이언트 패킷 처리 큐, 락 프리 큐이므로 임계구역 사용 안해도 됨
extern CRITICAL_SECTION ThreadSection; // 임계구역

// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg);

// 소켓 함수 오류 출력
void err_display(const char* msg);

// 소켓 함수 오류 출력
void err_display(int errcode);