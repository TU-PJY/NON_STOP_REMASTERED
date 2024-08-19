#include "Player.h"
#include "Framework.h"
#include <cmath>

void Player::InputKey(KeyType Type, KeyState State, unsigned char NormalKey, int SpecialKey) {
	switch (Type) {
	case KeyType::NormalKey:
		switch (State) {
		case KeyState::Down:
			switch (NormalKey) {
				// 죄우이동 활성화
			case 'a':
				MoveLeft = true;
				break;
			case 'd':
				MoveRight = true;
				break;

				// 스페이스바
				// 점프 활성화
			case 32:
				// 플레이어가 땅 위에 있을 때만 점프 가능
				if(psUtil.IsOnFloor(Height, FLOOR_HEIGHT))
					// 점프 가속값 추가, 떨어지는 상태로 변경
					psUtil.AddGravityAcc(5.0);
				break;
			}
			break;

		case KeyState::Up:
			switch(NormalKey) {
				// 좌우 이동 비활성화
			case 'a':
				MoveLeft = false;
				break;
			case 'd':
				MoveRight = false;
				break;
			}
			break;
		}
		break;
	}
}

GLfloat Player::GetPositionX() {
	return PositionX;
}

GLfloat Player::GetHeight() {
	return Height;
}

GLfloat Player::GetViewportPositionX() {
	return ViewportPositionX;
}

GLfloat Player::GetViewportHeight() {
	return ViewportHeight;
}

Player::Player() {
	SetImage(Image, "player");
}

void Player::UpdateViewportPosition() {
	// 화면 상의 좌표 업데이트
	ViewportPositionX = ViewportPosition().x;
	ViewportHeight = ViewportPosition().y;
}

void Player::UpdateLook() {
	if (auto Target = framework.Find("target"); Target) {
		// 플레이어 위치와 조준점 위치에 따라 보는 방향이 달라진다
		if (ViewportPositionX < Target->GetPositionX())
			LookDir = Right;
		else if (ViewportPositionX > Target->GetPositionX())
			LookDir = Left;
	}
}

void Player::UpdateAnimation(float FT) {
	// 움직이는 방향으로 몸을 기울인다
	if ((!MoveRight && !MoveLeft) || (MoveRight && MoveLeft)) {
		Rotation = std::lerp(Rotation, 0, FT * 10);
		ScaleValue = std::lerp(ScaleValue, 0, FT * 10);
	}

	else {
		if (MoveRight)
			Rotation = std::lerp(Rotation, -5, FT * 10);
		if (MoveLeft)
			Rotation = std::lerp(Rotation, 5, FT * 10);

		ScaleValue = std::lerp(ScaleValue, 0.03, FT * 10);
	}

	// 보는 방향이 바뀔경우 보는 방향으로 회전한다
	if (LookDir == Right)
		H_Rotation = std::lerp(H_Rotation, 0, FT * 10);
	else if (LookDir == Left)
		H_Rotation = std::lerp(H_Rotation, 180, FT * 10);

	Size = lsAni.Update(ScaleValue, FT * 10);
}

void Player::UpdateMove(float FT) {
	// A 또는 D 누를 시 가속, 두 키중 하나도 누르지 않거나 두 키를 모두 누를 시 감속
	if ((!MoveRight && !MoveLeft) || (MoveRight && MoveLeft))
		psUtil.LinearDcc(CurrentPlayerSpeed, 10, FT);
	else {
		if (MoveRight)
			psUtil.LinearAcc(CurrentPlayerSpeed, 1, MaxPlayerSpeed, 10, FT);
		if (MoveLeft)
			psUtil.LinearAcc(CurrentPlayerSpeed, -1, MaxPlayerSpeed, 10, FT);
	}

	// 플레이어 속도가 0 미만이거나 초과일 시 이동
	MoveStraight(PositionX, CurrentPlayerSpeed, FT);
}

void Player::UpdateJump(float FT) {
	// 점프 가속도가 추가된 후에는 중력이 작용
	psUtil.Fall(Height, 10.0, FT);

	// 플레이어 높이가 -0.3에 도달하는지 검사, 도달 시 점프 가속도가 0이 되어 중력 작용 중단
	// FLOOR_HEIGHT는 EngineHeader.h에 constexpr로 정의되어있음, 참조.
	psUtil.LandFloor(Height, FLOOR_HEIGHT);
}

void Player::Update(float FT) {
	// 플레이어 이동 업데이트
	UpdateMove(FT);

	// 플레이어 점프 업데이트
	UpdateJump(FT);

	// 플레이어 시점 업데이트
	UpdateLook();

	// 플레이어 애니메이션 업데이트
	UpdateAnimation(FT);
}

void Player::Render() {
	BeginProcess(ImageRenderMode::Default);
	SetPosition(PositionX, Height + Size * 0.5);
	RotateAxis(Rotation, 0.0, -0.1);
	RotateVertical(H_Rotation);
	Scale(0.3, 0.3 + Size);
	RenderImage(Image);

	SetPosition(0.0, -Size * 0.5);
	UpdateViewportPosition();
}