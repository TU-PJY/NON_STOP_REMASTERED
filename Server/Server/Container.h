#pragma once
#include "Packet.h"

extern std::vector<ClientInfo*> ConnectedClients; // 연결된 클라이언트 목록
extern boost::lockfree::queue<ClientPacketInfo> ClientPacketQueue; // 클라이언트 패킷 처리 큐, 락 프리 큐이므로 임계구역 사용 안해도 됨
extern CRITICAL_SECTION ThreadSection; // 임계구역
extern ClientPacketInfo Q_PacketInfo; // 다른 클라이언트로 보낼 패킷 정보