#pragma once
#pragma once
#include "TCP_header.h"

// 패킷 타입 열거형 모음, 반드시 서버와 클라가 동일한 것을 써야함
enum PacketTypeEnum {
	PACKET_TYPE_NONE, // 타입 없음
	PACKET_TYPE_ENTER, // 로비 패킷
	PACKET_TYPE_PLAYER, // 플레이어 움직임
	PACKET_TYPE_PLAYER_DELETE, // 플레이어 제거
	PACKET_TYPE_MONSTER_ADD, // 몬스터 추가
	PACKET_TYPE_MONSTER_DAMAGE, // 몬스터에게 가한 대미지
	PACKET_TYPE_MONSTER_DELETE // 몬스터 제거
};

////////////////////////// 패킷 구조체 모음

#pragma pack(push, 1)
// 로비 (클라 -> 서버)
typedef struct {
	char PlayerTag[20];    // 플레이어 이름
	char GunType[20];      // 무기 타입
}CS_INFO_PACKET;

// 로비 (서버 -> 클라)
typedef struct {
	char PlayerTag[20];    // 플레이어 이름
	char GunType[20];      // 무기 타입
}SC_INFO_PACKET;

typedef struct { // 플레이어 이동
	char PlayerTag[20];   // 닉네임
	float x, y;                      // 위치
	int LookDir;                // 방향 (0: 왼쪽, 1: 오른쪽)
	float GunRotation;          // 무기 회전 값
	float RecoilPosition; // 무기 반동 위치
	int HP; // 플레이어 체력
}CS_PLAYER_PACKET;

typedef struct { // 플레이어 이동
	char PlayerTag[20];  // 닉네임
	float x, y;                    // 위치
	int LookDir;              // 방향 (0: 왼쪽, 1: 오른쪽)
	float GunRotation;        // 무기 회전 값
	float RecoilPosition; // 무기 반동 위치
	int HP; // 플레이어 체력
}SC_PLAYER_PACKET;

typedef struct { // 몬스터 추가 
	int AddDir; // 추가되는 방향
	int ID; // 아이디
	char TrackTag[20];
}SC_MONSTER_ADD_PACKET;

typedef struct { // 죽은 몬스터 아이디 (클라 -> 서버)
	int ID;
}CS_MONSTER_DELETE_PACKET;

typedef struct { // 죽은 몬스터 아이디 (서버 -> 클라)
	int ID;
}SC_MONSTER_DELETE_PACKET;

typedef struct { // 몬스터 대미지 (클라->서버)
	int ID;
	int Damage;
}CS_MONSTER_DAMAGE_PACKET;

typedef struct { // 몬스터 대미지 (서버->클라)
	int ID;
	int Damage;
}SC_MONSTER_DAMAGE_PACKET;

// 플레이어 사망 패킷
typedef struct {
	char DeadPlayerTag[20];
}CS_PLAYER_DELETE_PACKET;

typedef struct {
	char DeadPlayerTag[20];
}SC_PLAYER_DELETE_PACKET;

#pragma pack(pop)
