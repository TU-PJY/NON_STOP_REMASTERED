#pragma once
#include "Framework.h"

enum PlayerLook
{Left, Right};

class Player : public GameObject {
private:
	Image Image;
	PhysicsUtil psUtil;
	LoopSinAnimation lsAni;

	GLfloat PositionX{0.0}, Height{ FLOOR_HEIGHT };
	GLfloat CurrentPlayerSpeed{};
	GLfloat MaxPlayerSpeed{ 1.8 };

	bool MoveRight{}, MoveLeft{};
	bool JumpActivated{};

	GLfloat Rotation{};
	GLfloat H_Rotation{};
	GLfloat Size{};
	GLfloat ScaleValue{};

	GLfloat ViewportPositionX{}, ViewportHeight{};

	int LookDir{ Right };

public:
	Player();
	void UpdateViewportPosition();
	void UpdateLook();
	void UpdateAnimation(float FT);
	void UpdateMove(float FT);
	void InputKey(KeyType Type, KeyState State, unsigned char NormalKey, int SpecialKey);
	GLfloat GetPositionX();
	GLfloat GetHeight();
	GLfloat GetViewportPositionX();
	GLfloat GetViewportHeight();
	int GetLookDir();
	void UpdateJump(float FT);
	void Update(float FT);
	void Render();
};

