#pragma once
#include "GameObject.h"

enum PlayerLook
{Left, Right};

class Player : public GameObject {
private:
	Image Image;
	PhysicsUtil psUtil;

	GLfloat PositionX{0.0}, Height{ FLOOR_HEIGHT };
	GLfloat CurrentPlayerSpeed{};
	GLfloat MaxPlayerSpeed{ 1.5 };

	bool MoveRight{}, MoveLeft{};
	bool JumpActivated{};

public:
	Player();
	void UpdateMove(float FT);
	void InputKey(KeyType Type, KeyState State, unsigned char NormalKey, int SpecialKey);
	void UpdateJump(float FT);
	void Update(float FT);
	void Render();
};

