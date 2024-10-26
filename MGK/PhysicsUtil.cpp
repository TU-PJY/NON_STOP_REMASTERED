#include "PhysicsUtil.h"
#include "MathUtil.h"
#include "EXUtil.h"

void PhysicsUtil::SetGravity(GLfloat Value) {
	if (Value < 0.0) return;
	Gravity = Value;
}

void PhysicsUtil::SetFriction(GLfloat Value) {
	if (Value < 0.0) return;
	Friction = Value;
}

void PhysicsUtil::SetRebounceReduce(GLfloat Value) {
	if (Value < 0.0) return;
	RebounceReduce = Value;
}

void PhysicsUtil::SetMinimumRebounce(GLfloat Value) {
	if (Value < 0.0) return;
	MinRebounceValue = Value;
}

void PhysicsUtil::SetFloorHeight(GLfloat Value) {
	FloorHeight = Value;
}

void PhysicsUtil::SetHeightOffset(GLfloat Value) {
	HeightOffset = Value;
}

bool PhysicsUtil::GetFallingState() {
	return FallingState;
}

void PhysicsUtil::AddGravityAcc(GLfloat Value) {
	GravityAcc += Value;
	FallingState = true;
}

void PhysicsUtil::SetGravityAcc(GLfloat Value) {
	GravityAcc = Value;
	FallingState = true;
}

void PhysicsUtil::EnableFalling() {
	FallingState = true;
}

void PhysicsUtil::DisableFalling() {
	FallingState = false;
}

bool PhysicsUtil::CheckFloorCollision(GLfloat& HeightPosition) {
	if (HeightPosition <= FloorHeight + HeightOffset)
		return true;
	return false;
}

void PhysicsUtil::LandOnFloor(GLfloat& HeightPosition) {
	HeightPosition = FloorHeight + HeightOffset;
	FallingState = false;
}

void PhysicsUtil::UpdateFalling(GLfloat& HeightPosition, float FrameTime) {
	if (FallingState) {
		GravityAcc -= Gravity * FrameTime;
		HeightPosition += GravityAcc * FrameTime;
		if (CheckFloorCollision(HeightPosition))
			LandOnFloor(HeightPosition);
	}
}

void PhysicsUtil::UpdateBouncing(GLfloat& HeightPosition, float FrameTime) {
	if (FallingState) {
		GravityAcc -= Gravity * FrameTime;
		HeightPosition += GravityAcc * FrameTime;
		if (CheckFloorCollision(HeightPosition)) {
			if (fabs(GravityAcc) <= MinRebounceValue)
				LandOnFloor(HeightPosition);
			else {
				HeightPosition = FloorHeight + HeightOffset;
				GravityAcc *= -1;
				GravityAcc -= RebounceReduce;
			}
		}
	}
}

void PhysicsUtil::LerpAcceleratation(GLfloat& Speed, GLfloat DestSpeed, GLfloat AccValue, float FT) {
	Speed = Math::Lerp(Speed, DestSpeed, AccValue * (1.0 - Friction), FT);
}

void PhysicsUtil::LerpDeceleration(GLfloat& Speed, float FT) {
	Speed = Math::Lerp(Speed, 0.0, Friction, FT);
}

void PhysicsUtil::LinearAcceleratation(GLfloat& Speed, GLfloat DestSpeed, GLfloat AccValue, float FT) {
	if (DestSpeed > 0.0) {
		Speed += AccValue * (1.0 - Friction) * FT;
		EX::ClampValue(Speed, DestSpeed, CLAMP_GREATER);
	}
	else {
		Speed -= AccValue * (1.0 - Friction) * FT;
		EX::ClampValue(Speed, DestSpeed, CLAMP_LESS);
	}
}

void PhysicsUtil::LinearDeceleration(GLfloat& Speed, float FT) {
	if (Speed > 0) {
		Speed -= Friction * FT;
		EX::ClampValue(Speed, 0.0, CLAMP_LESS);
	}
	else if (Speed < 0) {
		Speed += Friction * FT;
		EX::ClampValue(Speed, 0.0, CLAMP_GREATER);
	}
}

void PhysicsUtil::Adjust2SpeedEqual(GLfloat& SpeedX, GLfloat& SpeedY) {
	GLfloat Length = std::sqrt(SpeedX * SpeedX + SpeedY * SpeedY);
	if (Length > 1.0) {
		SpeedX /= Length;
		SpeedY /= Length;
	}
}