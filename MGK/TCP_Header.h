#pragma once
#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���� ���� API ��� �� ��� ����
#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ

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

// Ŭ���̾�Ʈ ����
typedef struct {
    SOCKET ClientSocket;
    struct sockaddr_in ClientAddr;
    char Addr[INET_ADDRSTRLEN];  // Ŭ���̾�Ʈ ip �ּ� ���ڿ�
} ClientInfo;

// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg);

// ���� �Լ� ���� ���
void err_display(const char* msg);

// ���� �Լ� ���� ���
void err_display(int errcode);

extern char SERVER_IP[20]; // Ŭ���̾�Ʈ �ּ�
extern CRITICAL_SECTION ThreadSection;

extern std::vector<std::string> ConnectedPlayer;
extern int NumPlayerConnected;

// �÷��̾� ���� ���� ���⿡ ����
extern bool ConnectState;
extern std::string PlayerGunType;
extern std::string PlayerTag;
extern bool PlayerReadyState;

// ����� ���� ���� ID�� �����
typedef struct {
    int ID;
    int Damage;
}DamageInfo;

// ���� ���� ����Ʈ
extern std::vector<int> DeleteMonsterList;

// ����� ���� ���� ����Ʈ
extern std::vector<DamageInfo> DamageMonsterList;