#pragma once
#include "EngineHeader.h"
#include <cmath>

class PhysicsUtil {
private:
	GLfloat Gravity{};
	GLfloat Friction{};
	GLfloat RebounceReduce{};
	GLfloat MinRebounceValue{};

	GLfloat FloorHeight{};
	GLfloat HeightOffset{};

	GLfloat GravityAcc{};
	GLfloat MoveAcc{};

	bool FallingState{};

public:
	bool GetFallingState();
	void EnableFalling();
	void DisableFalling();
	void AddGravityAcc(GLfloat AccValue);
	void SetGravityAcc(GLfloat AccValue);
	void SetGravity(GLfloat Value);
	void SetFloorHeight(GLfloat Value);
	void SetFriction(GLfloat Value);
	void SetHeightOffset(GLfloat Value);
	void SetRebounceReduce(GLfloat Value);
	void SetMinimumRebounce(GLfloat Value);
	bool CheckFloorCollision(GLfloat& Position);
	void LandOnFloor(GLfloat& Position);
	void UpdateFalling(GLfloat& HeightPosition, float FrameTime);
	void UpdateBouncing(GLfloat& HeightPosition, float FrameTime);
	void LerpAcceleratation(GLfloat& Speed, GLfloat DestSpeed, GLfloat AccValue, float FT);
	void LerpDeceleration(GLfloat& Speed, float FT);
	void LinearAcceleratation(GLfloat& Speed, GLfloat DestSpeed, GLfloat AccValue, float FT);
	void LinearDeceleration(GLfloat& Speed, float FT);
	void Adjust2SpeedEqual(GLfloat& SpeedX, GLfloat& SpeedY);
};