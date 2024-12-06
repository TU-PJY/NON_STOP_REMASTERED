#include "Player.h"
#include "MouseUtil.h"
#include "CameraCOntroller.h"
#include "Scene.h"

#include "SCAR_H.h"
#include "M16.h"
#include "MP44.h"
#include "MG42.h"

void Player::InputKey(int State, unsigned char NormalKey, int SpecialKey) {
	switch (State) {
	case NORMAL_KEY_DOWN:
		switch (NormalKey) {
		case 'a': MoveLeft = true; break;
		case 'd': MoveRight = true; break;

		case 'r':
			GunPtr->Reload();
			break;

		case NK_SPACE: 
			if (!JumpState) {
				pUtil.SetGravityAcc(6.0);
				JumpState = true;
				LandState = false;
			}
			break;
		}
		break;

	case NORMAL_KEY_UP:
		switch (NormalKey) {
		case 'a': MoveLeft = false; break;
		case 'd': MoveRight = false; break;
		}
		break;
	}
}

void Player::InputMouse(int State) {
	switch (State) { // ��Ŭ�� �� �ݹ�, ��Ŭ�� ���� �� �ݹ� ����
	case LEFT_BUTTON_DOWN: TriggerState = true; break;
	case LEFT_BUTTON_UP: TriggerState = false; break;
	}
}

AABB Player::GetAABB() {
	return aabb;
}

int Player::GetLookDir() {
	return LookDir;
}

GLfloat Player::GetGunRotation() {
	return GunRotation;
}

GLfloat Player::GetRecoilPosition() {
	if (GunPtr)
		return GunPtr->GetRecoilPosition();
	return 0.0;
}


void Player::GiveDamage(int Damage) {
	if (HurtEnableState) {
		HP -= Damage;
		EX::ClampValue(HP, 0, CLAMP_LESS);

		IndHPPtr->InputCurrentHP(HP);
		HurtTimer.Reset();

		// ī�޶� ��鸲 �߰�
		cameraCon.AddShakeValue(3.0);

		HurtEnableState = false;
	}
}

Player::Player(std::string Name) {
	// ���������� �߷°� �ٴ� ��ġ ����
	pUtil.SetGravity(14);
	pUtil.SetFloorHeight(-0.5);

	GunType = Name;

	// ü�� �ε������� ������Ʈ ������ ���
	IndHPPtr = scene.Find("indhp");
	IndHPPtr->InputCurrentHP(HP);
}

void Player::AddGunObject() {
	// �ڱⰡ ���� �� �̸��� ���� �ٸ� ���� scene�� �߰��ȴ�.
	if (GunType == "SCAR_H")
		scene.AddObject(new SCAR_H, GunType.c_str(), LAYER_3);

	else if(GunType == "M16")
		scene.AddObject(new M16, GunType.c_str(), LAYER_3);

	else if (GunType == "MP44")
		scene.AddObject(new MP44, GunType.c_str(), LAYER_3);

	else if (GunType == "MG42")
		scene.AddObject(new MG42, GunType.c_str(), LAYER_3);

	// �� ������ ����
	GunPtr = scene.Find(GunType.c_str());
}

void Player::UpdateGun() {
	if (!GunPtr)
		AddGunObject();

	// �� ������Ʈ �����Ͱ� �ִٸ� ������ �÷��̾��� ������ ������. (��ġ, ����, ��Ƽ� ��� ��)
	if (GunPtr) {
		GunPtr->InputPosition(Position);
		GunPtr->InputLookDir(LookDir);
		GunRotation = Math::CalcDegree(Position.x, Position.y, mouse.x - cameraCon.Position.x, mouse.y);
		GunPtr->InputRotation(GunRotation);
		GunPtr->InputTriggerState(TriggerState);
	}
}

void Player::UpdateHurtCoolTime(float FT) {
	if (!HurtEnableState)
		HurtTimer.Update(FT);
	if (HurtTimer.MiliSec(2) >= HurtCoolTime)
		HurtEnableState = true;
}

void Player::UpdateHeal(float FT) {
	if(HP < 100)
		HealTimer.Update(FT);

	if (HealTimer.MiliSec(2) >= HealCoolTime) {
		HealTimer.Interpolate(HealCoolTime);
		HP += 1;

		if (HP == 100)
			HealTimer.Reset();

		IndHPPtr->InputCurrentHP(HP);
	}
}

void Player::UpdateFunc(float FT) {
	// ������ ���ο� ���� �̵� ������ �޶�����.
	// ���� ���� ������ �ʵ��� �Ѵ�.
	if (MoveRight) {
		Position.x += Speed * FT;
		if (Position.x > 7.5)
			Position.x = 7.5;
	}

	if (MoveLeft) {
		Position.x -= Speed * FT;
		if (Position.x < -7.5)
			Position.x = -7.5;
	}

	// ���콺 ��ġ�� ���� �ٶ󺸴� ������ �޶�����.
	// ���콺 ��ġ�� ī�޶� ��ġ�� ���Խ��Ѿ� �Ѵ�.
	if      (mouse.x - cameraCon.Position.x > Position.x)  LookDir = LOOK_RIGHT;
	else if (mouse.x - cameraCon.Position.x < Position.x)  LookDir = LOOK_LEFT;

	// �÷��̾� ���� ������Ʈ
	pUtil.UpdateFalling(Position.y, FT);
	if (!pUtil.GetFallingState() && !LandState) {
		// ī�޶� �����ϴ� ȿ���� �ο��Ѵ�.
		cameraCon.Push(0.2);
		JumpState = false;
		LandState = true;
	}

	// �� ������Ʈ
	UpdateGun();

	// ����� �Ա� ��Ÿ�� ������Ʈ
	UpdateHurtCoolTime(FT);

	// ȸ�� ������Ʈ
	UpdateHeal(FT);

	// AABB ������Ʈ
	aabb.Update(Position.x, Position.y, 0.25, 0.3);
}

void Player::RenderFunc() {
	// ��� �ʱ�ȭ, ������ �� �ݵ�� �����ؾ��Ѵ�.
	InitMatrix();

	// �÷��̾� ��ġ ����ŭ �̵�
	Transform::Move(TranslateMatrix, Position);

	// �÷��̾� ��� ũ�� ����
	Transform::Scale(ScaleMatrix, 0.3, 0.3);

	// ������ �ٶ󺸴� ��� ����������� �̹��� ����
	if (LookDir == LOOK_LEFT)
		Flip(FLIP_H);

	// �������� �ʿ��� �����͵��� ���̴��� ���� �� ���� ������
	Render(PlayerImage);

	// AABB ������
	aabb.Render();
}

glm::vec2 Player::GetPosition() {
	return Position;
}
