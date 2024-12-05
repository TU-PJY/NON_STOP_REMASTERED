#pragma once
#include "Packet.h"

extern std::vector<ClientInfo*> ConnectedClients; // ����� Ŭ���̾�Ʈ ���
extern boost::lockfree::queue<InputPacketInfo> ClientPacketQueue; // Ŭ���̾�Ʈ ��Ŷ ó�� ť, �� ���� ť�̹Ƿ� �Ӱ豸�� ��� ���ص� ��
extern CRITICAL_SECTION ThreadSection; // �Ӱ豸��