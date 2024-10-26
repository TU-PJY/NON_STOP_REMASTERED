#include "Player.h"
#include "MouseUtil.h"
#include "CameraCOntroller.h"
#include "Scene.h"

#include "SCAR_H.h"


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
	switch (State) { // 좌클릭 시 격발, 좌클릭 해제 시 격발 해제
	case LEFT_BUTTON_DOWN: TriggerState = true; break;
	case LEFT_BUTTON_UP: TriggerState = false; break;
	}
}

void Player::UpdateGun() {
	// 자기가 가진 총 이름에 따라 다른 총이 scene에 추가된다.
	if (!GunPtr) {
		if (GunName == "SCAR_H")
			scene.AddObject(new SCAR_H(0.13), GunName, LAYER_3);

		GunPtr = scene.Find(GunName);
	}

	// 총 오브젝트 포인터가 있다면 총으로 플레이어의 정보를 보낸다. (위치, 각도, 방아쇠 당김 등)
	if (GunPtr) {
		GunPtr->InputPosition(Position);
		GunPtr->InputLookDir(LookDir);
		GunPtr->InputRotation(Math::CalcDegree(Position.x, Position.y, mouse.x - cameraCon.Position.x, mouse.y));
		GunPtr->InputTriggerState(TriggerState);
	}
}

void Player::UpdateFunc(float FT) {
	// 움직임 여부에 따라 이동 방향이 달라진다.
	// 범위 밖을 나가지 않도록 한다.
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

	// 마우스 위치에 따라 바라보는 방향이 달라진다.
	// 마우스 위치에 카메라 위치를 포함시켜야 한다.
	if      (mouse.x - cameraCon.Position.x > Position.x)  LookDir = LOOK_RIGHT;
	else if (mouse.x - cameraCon.Position.x < Position.x)  LookDir = LOOK_LEFT;

	// 플레이어 점프 업데이트
	pUtil.UpdateFalling(Position.y, FT);
	if (!pUtil.GetFallingState())
		JumpState = false;

	UpdateGun();

	// AABB 업데이트
	aabb.Update(Position.x, Position.y, 0.25, 0.3);
}

void Player::RenderFunc() {
	// 행렬 초기화, 렌더링 전 반드시 실행해야한다.
	InitMatrix();

	// 플레이어 위치 값만큼 이동
	Transform::Move(TranslateMatrix, Position);

	// 플레이어 출력 크기 조정
	Transform::Scale(ScaleMatrix, 0.3, 0.3);

	// 왼쪽을 바라보는 경우 수평방향으로 이미지 반전
	if (LookDir == LOOK_LEFT) {
		if (JumpState)
			Transform::Rotate(RotateMatrix, 20.0);
		Flip(FLIP_H);
	}

	else if (LookDir == LOOK_RIGHT) {
		if (JumpState)
			Transform::Rotate(RotateMatrix, -20.0);
	}

	// 렌더링에 필요한 데이터들을 쉐이더로 전달 후 최종 렌더링
	Render(PlayerImage);

	// AABB 렌더링
	aabb.Render();
}

glm::vec2 Player::GetPosition() {
	return Position;
}
