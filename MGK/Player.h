#pragma once
#include "GameObject.h"

enum EnumLookDir{
	LOOK_LEFT,
	LOOK_RIGHT
};

// GameObject Ŭ�����κ��� ���
class Player : public GameObject {
private:
	// �÷��̾� ��ġ
	glm::vec2 Position{0.0, -0.5};

	// �÷��̾� �̵� �ӵ�
	GLfloat Speed{ 2.5 };

	// �÷��̾� �̵� ����
	int LookDir{};

	// �� ȸ�� ����
	GLfloat GunRotation{};

	// �÷��̾� �̵� ����
	bool MoveLeft{}, MoveRight{};

	// ���� ������ ���� ��������
	PhysicsUtil pUtil;

	// �÷��̾� ���� ����
	bool JumpState{};

	// �÷��̾� �� ���� ���� ����
	bool LandState{true };

	// �÷��̾� ��Ƽ� ��� ����
	bool TriggerState{};

	// �÷��̾ ����ִ� �� �̸�
	std::string GunType{};

	// �÷��̾ ����ִ� �� ������Ʈ ������
	GameObject* GunPtr{};

	// �÷��̾� hp �ε������� ������
	GameObject* IndHPPtr{};

	// AABB
	AABB aabb;

	// ü��
	int HP{ 100 };

	// ����� ��Ÿ��
	TimerUtil HurtTimer;
	GLfloat HurtCoolTime{ 0.7 };

	// ����� ���� �� �ִ� ����
	bool HurtEnableState{true};

	// ȸ�� ��Ÿ��
	TimerUtil HealTimer;
	GLfloat HealCoolTime{ 0.5 };

public:
	Player(std::string Name);

	int GetLookDir();

	GLfloat GetGunRotation();

	GLfloat GetRecoilPosition();

	// Ű �Է� ��Ʈ�ѷ�
	void InputKey(int State, unsigned char NormalKey, int SpecialKey);

	// ���콺 �Է� ��Ʈ�ѷ�
	void InputMouse(int State);

	AABB GetAABB();

	void AddGunObject();

	void UpdateGun();

	void UpdateHurtCoolTime(float FT);

	void UpdateHeal(float FT);

	void GiveDamage(int Value);

	// ������Ʈ �Լ�
	// FT�� ������Ÿ��. SceneŬ�����κ��� �޴´�.
	void UpdateFunc(float FT);

	// ������ �Լ�
	void RenderFunc();

	// �ٸ� ��ü �Ǵ� ��ġ���� �÷��̾��� ��ġ ���� ���� �� �ֵ��� �Ѵ�.
	glm::vec2 GetPosition();
};