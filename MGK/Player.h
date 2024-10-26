#pragma once
#include "GameObject.h"

enum EnumLookDir{
	LOOK_RIGHT,
	LOOK_LEFT
};

// GameObject 클래스로부터 상속
class Player : public GameObject {
private:
	// 플레이어 위치
	glm::vec2 Position{0.0, -0.5};

	// 플레이어의 뷰포트 기준 위치
	glm::vec2 ViewportPosition{};

	// 플레이어 이동 속도
	GLfloat Speed{ 2.5 };

	// 플레이어 이동 방향
	int LookDir{};

	// 플레이어 이동 여부
	bool MoveLeft{}, MoveRight{};

	// 점프 구현을 위한 물리엔진
	PhysicsUtil pUtil;

	// 플레이어 점프 여부
	bool JumpState{};

public:
	Player();

	// 키 입력 컨트롤러
	void InputKey(int State, unsigned char NormalKey, int SpecialKey);

	// 마우스 입력 컨트롤러
	void InputMouse(int State);

	// 업데이트 함수
	// FT는 프레임타임. Scene클래스로부터 받는다.
	void UpdateFunc(float FT);

	// 렌더링 함수
	void RenderFunc();

	// 다른 객체 또는 위치에서 플레이어의 위치 값을 얻을 수 있도록 한다.
	glm::vec2 GetPosition();
};