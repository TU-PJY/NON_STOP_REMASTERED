#include "TCP_Header.h"
#include "Packet.h"

std::vector<ClientInfo*> ConnectedClients; // ����� Ŭ���̾�Ʈ ���
std::vector<std::string> NameList;
boost::lockfree::queue<InputPacketInfo> ClientPacketQueue; // Ŭ���̾�Ʈ ��Ŷ ó�� ť, �� ���� ť�̹Ƿ� �Ӱ豸�� ��� ���ص� ��
CRITICAL_SECTION ThreadSection; // �Ӱ豸��