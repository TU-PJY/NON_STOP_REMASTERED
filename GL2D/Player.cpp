#include "Player.h"

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
					psUtil.AddGravityAcc(4.0);
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

Player::Player() {
	SetImage(Image, "player");
}

void Player::UpdateMove(float FT) {
	// A 또는 D 누를 시 가속, 두 키중 하나도 누르지 않거나 두 키를 모두 누를 시 감속
	if (MoveRight)
		psUtil.LinearAcc(CurrentPlayerSpeed, 1, MaxPlayerSpeed, 10, FT);

	if (MoveLeft)
		psUtil.LinearAcc(CurrentPlayerSpeed, -1, MaxPlayerSpeed, 10, FT);

	if ((!MoveRight && !MoveLeft) || (MoveRight && MoveLeft))
		psUtil.LinearDcc(CurrentPlayerSpeed, 10, FT);

	// 플레이어 속도가 0 미만이거나 초과일 시 이동
	MoveStraight(PositionX, CurrentPlayerSpeed, FT);
}

void Player::UpdateJump(float FT) {
	// 점프 가속도가 추가된 후에는 중력이 작용
	psUtil.Fall(Height, 9.0, FT);

	// 플레이어 높이가 -0.3에 도달하는지 검사, 도달 시 점프 가속도가 0이 되어 중력 작용 중단
	// FLOOR_HEIGHT는 EngineHeader.h에 constexpr로 정의되어있음, 참조.
	psUtil.LandFloor(Height, FLOOR_HEIGHT);
}

void Player::Update(float FT) {
	// 플레이어 이동 업데이트
	UpdateMove(FT);

	// 플레이어 점프 업데이트
	UpdateJump(FT);
}

void Player::Render() {
	BeginProcess(ImageRenderMode::Default);
	SetPosition(PositionX, Height);
	Scale(0.3, 0.3);
	RenderImage(Image);
}