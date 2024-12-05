#pragma once
#pragma once
#include "TCP_header.h"

// 패킷 타입 열거형 모음, 반드시 서버와 클라가 동일한 것을 써야함
enum PacketTypeEnum {
	PACKET_TYPE_NONE, // 타입 없음
	PACKET_TYPE_ENTER, // 로비 패킷
	PACKET_TYPE_MOVE, // 플레이어 움직임
	PACKET_TYPE_PLAYER_ADD, // 플레이어 추가 패킷
	PACKET_TYPE_PLAYER_DELETE, // 플레이어 제거 패킷
};

////////////////////////// 패킷 구조체 모음

#pragma pack(push, 1)
// 로비 (클라 -> 서버)
typedef struct {
	char PlayerTag[20];    // 플레이어 이름
	char GunType[20];      // 무기 타입
}CS_LOBBY_PACKET;

// 로비 (서버 -> 클라)
typedef struct {
	char PlayerTag[20];    // 플레이어 이름
	char GunType[20];      // 무기 타입
}SC_INFO_PACKET;

typedef struct { // 플레이어 이동
	char PlayerTag[20];   // 닉네임
	float x, y;                      // 위치
	int look_dir;                // 방향 (0: 왼쪽, 1: 오른쪽)
	float gun_rotation;          // 무기 회전 값
}CS_PLAYER_MOVE_PACKET;

typedef struct { // 플레이어 이동
	char PlayerTag[20];  // 닉네임
	float x, y;                    // 위치
	int look_dir;              // 방향 (0: 왼쪽, 1: 오른쪽)
	float gun_rotation;        // 무기 회전 값
}SC_PLAYER_MOVE_PACKET;


#pragma pack(pop)
