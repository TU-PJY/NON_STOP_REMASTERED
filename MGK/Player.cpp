#include "Player.h"
#include "MouseUtil.h"

Player::Player(){
	// 물리엔진의 중력과 바닥 위치 지정
	pUtil.SetGravity(15);
	pUtil.SetFloorHeight(-0.5);
}

void Player::InputKey(int State, unsigned char NormalKey, int SpecialKey) {
	switch (State) {
	case NORMAL_KEY_DOWN:
		switch (NormalKey) {
		case 'a': MoveLeft = true; break;
		case 'd': MoveRight = true; break;
		case NK_SPACE: 
			if (!JumpState) {
				pUtil.SetGravityAcc(6.0);
				JumpState = true;
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

}

void Player::UpdateFunc(float FT) {
	// 움직임 여부에 따라 이동 방향이 달라진다.
	if (MoveRight)  Position.x += Speed * FT;
	if (MoveLeft)   Position.x -= Speed * FT;

	// 마우스 위치에 따라 바라보는 방향이 달라진다.
	if      (mouse.x > Position.x)  LookDir = LOOK_RIGHT;
	else if (mouse.x < Position.x)  LookDir = LOOK_LEFT;

	// 플레이어 점프 업데이트
	pUtil.UpdateFalling(Position.y, FT);
	if (!pUtil.GetFallingState())
		JumpState = false;
}

void Player::RenderFunc() {
	// 행렬 초기화, 렌더링 전 반드시 실행해야한다.
	InitMatrix();

	// 플레이어 위치 값만큼 이동
	Transform::Move(TranslateMatrix, Position);

	// 플레이어 출력 크기 조정
	Transform::Scale(ScaleMatrix, 0.15, 0.15);

	// 왼쪽을 바라보는 경우 수평방향으로 이미지 반전
	if (LookDir == LOOK_LEFT)
		Flip(FLIP_H);

	// 렌더링에 필요한 데이터들을 쉐이더로 전달 후 최종 렌더링
	Render(PlayerImage);
}
