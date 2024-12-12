#pragma once
#pragma once
#include "TCP_header.h"

// ��Ŷ Ÿ�� ������ ����, �ݵ�� ������ Ŭ�� ������ ���� �����
enum PacketTypeEnum {
	PACKET_TYPE_NONE, // Ÿ�� ����
	PACKET_TYPE_ENTER, // �κ� ��Ŷ
	PACKET_TYPE_PLAYER, // �÷��̾� ������
	PACKET_TYPE_PLAYER_DELETE, // �÷��̾� ����
	PACKET_TYPE_MONSTER_ADD, // ���� �߰�
	PACKET_TYPE_MONSTER_DAMAGE, // ���Ϳ��� ���� �����
	PACKET_TYPE_MONSTER_DELETE // ���� ����
};

////////////////////////// ��Ŷ ����ü ����

#pragma pack(push, 1)
// �κ� (Ŭ�� -> ����)
typedef struct {
	char PlayerTag[20];    // �÷��̾� �̸�
	char GunType[20];      // ���� Ÿ��
}CS_INFO_PACKET;

// �κ� (���� -> Ŭ��)
typedef struct {
	char PlayerTag[20];    // �÷��̾� �̸�
	char GunType[20];      // ���� Ÿ��
}SC_INFO_PACKET;

typedef struct { // �÷��̾� �̵�
	char PlayerTag[20];   // �г���
	float x, y;                      // ��ġ
	int LookDir;                // ���� (0: ����, 1: ������)
	float GunRotation;          // ���� ȸ�� ��
	float RecoilPosition; // ���� �ݵ� ��ġ
	int HP; // �÷��̾� ü��
}CS_PLAYER_PACKET;

typedef struct { // �÷��̾� �̵�
	char PlayerTag[20];  // �г���
	float x, y;                    // ��ġ
	int LookDir;              // ���� (0: ����, 1: ������)
	float GunRotation;        // ���� ȸ�� ��
	float RecoilPosition; // ���� �ݵ� ��ġ
	int HP; // �÷��̾� ü��
}SC_PLAYER_PACKET;

typedef struct { // ���� �߰� 
	int AddDir; // �߰��Ǵ� ����
	int ID; // ���̵�
	char TrackTag[20];
}SC_MONSTER_ADD_PACKET;

typedef struct { // ���� ���� ���̵� (Ŭ�� -> ����)
	int ID;
}CS_MONSTER_DELETE_PACKET;

typedef struct { // ���� ���� ���̵� (���� -> Ŭ��)
	int ID;
}SC_MONSTER_DELETE_PACKET;

typedef struct { // ���� ����� (Ŭ��->����)
	int ID;
	int Damage;
}CS_MONSTER_DAMAGE_PACKET;

typedef struct { // ���� ����� (����->Ŭ��)
	int ID;
	int Damage;
}SC_MONSTER_DAMAGE_PACKET;

// �÷��̾� ��� ��Ŷ
typedef struct {
	char DeadPlayerTag[20];
}CS_PLAYER_DELETE_PACKET;

typedef struct {
	char DeadPlayerTag[20];
}SC_PLAYER_DELETE_PACKET;

#pragma pack(pop)
