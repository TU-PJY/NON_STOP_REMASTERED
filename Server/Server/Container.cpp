#include "TCP_Header.h"

std::vector<ClientInfo*> ConnectedClients; // 연결된 클라이언트 목록
boost::lockfree::queue<ClientPacketInfo> ClientPacketQueue; // 클라이언트 패킷 처리 큐, 락 프리 큐이므로 임계구역 사용 안해도 됨
CRITICAL_SECTION ThreadSection; // 임계구역