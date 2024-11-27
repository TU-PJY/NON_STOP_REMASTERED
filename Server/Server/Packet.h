#pragma once
#include "TCP_header.h"

// 패킷 타입 열거형 모음, 반드시 서버와 클라가 동일한 것을 써야함
enum PacketTypeEnum {
	PACKET_TYPE_NONE, // 타입 없음
	PACKET_TYPE_LOBBY, // 로비 패킷
	PACKET_TYPE_PLAYER_ADD, // 플레이어 추가 패킷
	PACKET_TYPE_PLAYER_DELETE, // 플레이어 제거 패킷
};

////////////////////////// 패킷 구조체 모음

#pragma pack(push, 1)
// 로비 (클라 -> 서버)
typedef struct {
	char PlayerTag[20];    // 플레이어 이름
	char GunType[20];      // 무기 타입
	bool ReadyState;       // 준비 상태
}CS_LOBBY_PACKET;

// 로비 (서버 -> 클라)
typedef struct {
	char PlayerTag[20];    // 플레이어 이름
	char GunType[20];      // 무기 타입
	bool ReadyState;       // 준비 상태
}SC_LOBBY_PACKET;

#pragma pack(pop)

// 큐에서 처리하는 서버 -> 클라이언트 구조체
typedef struct {
	ClientInfo* Client; // 클라이언트 정보
	uint8_t PacketType;  // 패킷 타입

	// 패킷 종류 추가 시 여기에도 추가해야 함
	// 패킷 타입에 따라 하나의 구조체 데이터만 선택해 전송함
	SC_LOBBY_PACKET SC_LobbyPacket;
} ClientPacketInfo;